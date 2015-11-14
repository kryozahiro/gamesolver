/*
 * Experiment.cpp
 *
 *  Created on: 2015/07/22
 *      Author: kryozahiro
 */

#include "Experiment.h"

#include <cmath>
#include <fstream>
#include <omp.h>
#include <boost/property_tree/xml_parser.hpp>
#include "cpputil/GenericIo.h"
#include "cpputil/OpenMp.h"
#include "cpputil/PropertyTreeUtil.h"
#include "cpputil/reflection/Reflection.h"

#include "../game/AverageAdaptor.h"
#include "../problem/HomoAdaptor.h"
#include "../problem/Regression.h"
#include "../problem/SantaFeTrail.h"
#include "../problem/TravellingSalesman.h"
using namespace std;
using namespace cpputil;
namespace po = boost::program_options;
namespace pt = boost::property_tree;
namespace lg = boost::log;

Experiment::Experiment(const boost::program_options::variables_map& args) : randomEngine(time(NULL)) {
	//実験の読み込み
	initExperiment(args);
}

void Experiment::operator()() {
	int stage = 0;
	std::vector<std::shared_ptr<Solution>> solutions;
	for (const pt::ptree::value_type& kvp : experiment) {
		//出力ファイルの生成
		output->setSink(stage, kvp.first);

		if (kvp.first == "Game") {
			pt::ptree gameTree = kvp.second;
			game = initGame(gameTree);

		} else if (kvp.first == "GeneratorStage") {
			pt::ptree generatorStageTree = kvp.second;
			generatorStage = make_shared<GeneratorStage>(config, generatorStageTree, game);
			generatorStage->operator()(solutions, randomEngine);

		} else if (kvp.first == "SolverStage") {
			pt::ptree solverStageTree = kvp.second;
			std::shared_ptr<SolverStage> solverStage = make_shared<SolverStage>(config, solverStageTree, game, generatorStage->getProgramName(), output->getEvaluationLoggerRange());
			solverStage->operator()(solutions, randomEngine);
			if (output->getRelationFile() != "") {
				ofstream ofs(output->getRelationFile());
				ofs << solverStage->getHistory()->showRelation() << endl;
			}

		} else if (kvp.first == "ValidatorStage") {
			pt::ptree validatorStageTree = kvp.second;
			validatorStage = make_shared<ValidatorStage>(config, validatorStageTree, game);
			validatorStage->operator()(solutions, randomEngine);
		}
		++stage;
	}
}

void Experiment::initExperiment(const boost::program_options::variables_map& args) {
	cerr << "Experiment:" << endl;

	//ファイルからの<Config>ノードの取得
	string configFile = args["config"].as<string>();
	pt::ptree configRoot;
	pt::read_xml(configFile, configRoot);
	config = configRoot.get_child("Config");

	//<Multithread>ノードの取得
	if (args["multithread"].as<int>() != 0) {
		initOpenMp(args["multithread"].as<int>());
	} else {
		initOpenMp(config.get<int>("Multithread"));
	}
	cerr << "threads = " << omp_get_max_threads() << endl;

	//<Experiment>ノードの取得
	if (args["experiment"].as<string>() != "") {
		experiment = cpputil::search(config, "<xmlattr>.name", args["experiment"].as<string>());
	} else {
		experiment = config.get_child("Experiment");
	}

	//出力方法
	pt::ptree outputTree;
	if (args["output"].as<string>() != "") {
		outputTree = cpputil::search(config, "<xmlattr>.name", args["output"].as<string>());
	} else {
		outputTree = experiment.get_child("Output");
	}
	outputTree = cpputil::solveReference(config, outputTree);
	output = make_shared<Output>(outputTree);

	cerr << endl;
}

shared_ptr<Game> Experiment::initGame(pt::ptree& gameTree) {
	gameTree = cpputil::solveReference(config, gameTree);
	gameTree.sort();
	string gameName = gameTree.rbegin()->first;
	pt::ptree concreteGameTree = gameTree.rbegin()->second;

	shared_ptr<Game> game;
	if (gameName == "AverageAdaptor") {
		//平均化
		pt::ptree implGameTree = concreteGameTree.get_child("Game");
		shared_ptr<Game> implGame = initGame(implGameTree);
		shared_ptr<AverageAdaptor> ave = make_shared<AverageAdaptor>(implGame, concreteGameTree.get<int>("SampleSize"));
		game = ave;

	} else if (gameName == "HomoAdaptor") {
		//均質化
		pt::ptree implGameTree = concreteGameTree.get_child("Game");
		shared_ptr<Game> implGame = initGame(implGameTree);
		shared_ptr<HomoAdaptor> homo = make_shared<HomoAdaptor>(implGame, implGame->getProgramSize().first);
		game = homo;

	} else if (gameName == "Regression") {
		//回帰問題
		shared_ptr<Regression> reg = make_shared<Regression>(concreteGameTree, randomEngine);
		game = reg;

	} else if (gameName == "LinerRegression") {
		//線形回帰問題
		/*ProgramType programType(ParameterType(-1, 1, 0.001), 1, ParameterType(-10, 10, 0.001), 1);
		shared_ptr<Regression> reg = make_shared<Regression>(programType);
		reg->addData(randomEngine, 20, [](const vector<double>& input) {
			double x = input[0];
			double y = 2.0 * x + 1.0;
			return vector<double>(1, y);
		});
		game = reg;*/
		throw "unimplemented";

	} else if (gameName == "SantaFeTrail") {
		//Santa Fe Trail
		shared_ptr<SantaFeTrail> sft = SantaFeTrail::getRegular();
		game = sft;

	} else if (gameName == "TravellingSalesman") {
		shared_ptr<TravellingSalesman> tsp = make_shared<TravellingSalesman>(10, time(NULL));
		game = tsp;

	} else {
		auto global = Reflection::get_mutable_instance().getGlobal();
		auto theClass = global->getScope(gameName);
		auto newOp = theClass->getFunction<Game*(const pt::ptree&, int)>("new");
		game = shared_ptr<Game>(newOp(concreteGameTree, time(NULL)));
	}

	cerr << "Game: " << gameName << endl;
	cerr << "type = " << game->getProgramType() << endl;
	cerr << game->toString();
	cerr << endl;
	return game;
}

/*vector<shared_ptr<Program>> Experiment::readPrograms(const Game& game, GameType gameType, ProgramType programType, istream& is) {
	vector<shared_ptr<Program>> programs;

	char c;
	is >> c;
	while (!is.eof() and (int)programs.size() < programNum) {
		is.unget();

		shared_ptr<Program> program;
		switch (programType) {
		case ProgramType::FEEDFORWARD_NETWORK: {
			FeedforwardNetwork net(game.getProgramType(), 20, [](double x){
				return FeedforwardNetwork::sigmoid(1.0, x);
			});
			is >> net;
			int inputSize = net.getInputEnd() - 1;
			int outputSize = net.getOutputEnd() - net.getHiddenEnd();
			net.insertNode(net.getInputEnd(), game.getProgramType().getInputLength() - inputSize, false);
			net.insertNode(net.getOutputEnd(), game.getProgramType().getOutputLength() - outputSize, false);

			//TODO: setup adjacency, changeable when insert

			program = make_shared<FeedforwardNetwork>(net);
		} break;
		case ProgramType::LAYERED_NETWORK: {
			vector<int> nodes = {0};
			LayeredNetwork net(game.getProgramType(), nodes, [](double x){
				return NeuralLayer::sigmoid(1.0,x);
			});
			is >> net;
			net.resizeLayers({game.getProgramType().getInputLength(), 7, game.getProgramType().getOutputLength()}, randomEngine);
			program = make_shared<LayeredNetwork>(net);
		} break;
		default:
			assert(false);
		}
		programs.push_back(program);
		is >> c;
	}
	assert((int)programs.size() == programNum);
	return programs;
}*/
