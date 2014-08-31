/*
 * AiBench.cpp
 *
 *  Created on: 2013/11/04
 *      Author: kryozahiro
 */

#include <fstream>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/expressions/predicates.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include "AiBench.h"
#include "Game/AverageAdaptor.h"
#include "Game/Mushroom.h"
#include "Game/Exchange.h"
#include "Game/Evasion.h"
#include "Game/TwoAgents.h"
#include "Problem/HomoAdaptor.h"
#include "Problem/Regression.h"
#include "Problem/TravellingSalesman.h"
#include "Problem/SantaFeTrail.h"
#include "Solver/GeneticAlgorithm.h"
#include "Solver/SimulatedAnnealing.h"
#include "Util/GenericIo.h"
#include "Util/PropertyTreeUtil.h"
using namespace std;
namespace opt = boost::program_options;
namespace prop = boost::property_tree;
namespace lg = boost::log;

AiBench::AiBench() : randomEngine(time(NULL)), logger(make_shared<lg::sources::logger>()) {
	lg::add_common_attributes();
}

void AiBench::initOptions(boost::program_options::options_description& options,
		boost::program_options::positional_options_description& positional) {
	options.add_options()
		("help,h", "produce help message")
		("config,c", opt::value<string>()->default_value("config.xml"), "specify config file")
		("experiment,e", opt::value<string>()->default_value(""), "specify experiment")
		("game,g", opt::value<string>()->default_value(""), "specify game")
		("program,p", opt::value<string>()->default_value(""), "specify program representation")
		("solver,s", opt::value<string>()->default_value(""), "specify solver")
		("agents,a", opt::value<int>()->default_value(0), "specify the size of agents")
		("times,t", opt::value<int>()->default_value(0), "specify evaluation times")
		("validation,v", opt::value<int>()->default_value(0), "specify validate evaluation times")
		("input,i", opt::value<string>()->default_value(""), "specify input file")
		("output,o", opt::value<string>()->default_value(""), "specify output methods");
}

int AiBench::mainImpl(boost::program_options::variables_map& args) {
	//実験の読み込み
	initExperiment(args);
	cerr << endl;

	//探索の実行
	cerr << "Solve: ";
	vector<shared_ptr<Solution>> solutions;
	Timer timer;
	timer.start();
	solutions = solver->solve(*game, evaluationTimes);
	cerr << (double)timer.elapsed() / 1000.0 << " sec elapsed" << endl;
	cerr << endl;

	//結果の出力
	cerr << "Summary:" << endl;
	cerr << history->showSummary() << endl;

	if (relationFile != "") {
		ofstream ofs(relationFile);
		ofs << history->showRelation() << endl;
	}

	cerr << "Solution:" << endl;
	cerr << solutions[0]->toString() << endl;

	//検証
	cerr << "Validation:" << endl;
	validate(solutions);
	return EXIT_SUCCESS;
}

void AiBench::initExperiment(boost::program_options::variables_map& args) {
	cerr << "initExperiment:" << endl;

	//ファイルからの<Config>ノードの取得
	string configFile = args["config"].as<string>();
	prop::ptree configRoot;
	prop::read_xml(configFile, configRoot);
	config = configRoot.get_child("Config");

	//<Experiment>ノードの取得
	prop::ptree experiment;
	if (args["experiment"].as<string>() != "") {
		experiment = PropertyTreeUtil::search(config, "<xmlattr>.name", args["experiment"].as<string>());
	} else {
		experiment = config.get_child("Experiment");
	}

	//プログラムのサイズ
	if (args["agents"].as<int>() != 0) {
		size = args["agents"].as<int>();
	} else {
		size = experiment.get<int>("Size");
	}
	cerr << "\tsize = " << size << endl;

	//評価回数
	if (args["times"].as<int>() != 0) {
		evaluationTimes = args["times"].as<int>();
	} else {
		evaluationTimes = experiment.get<int>("Evaluation");
	}
	cerr << "\tevaluation = " << evaluationTimes << endl;

	//検証
	if (args["validation"].as<int>() != 0) {
		validationTimes = args["validation"].as<int>();
	} else {
		validationTimes = experiment.get<int>("Validation");
	}
	cerr << "\tvalidation = " << validationTimes << endl;

	//ゲーム
	prop::ptree gameTree;
	if (args["game"].as<string>() != "") {
		gameTree = PropertyTreeUtil::search(config, "<xmlattr>.name", args["game"].as<string>());
	} else {
		gameTree = experiment.get_child("Game");
	}
	game = initGame(gameTree);

	//プログラム
	prop::ptree programTree;
	if (args["program"].as<string>() != "") {
		programTree = PropertyTreeUtil::search(config, "<xmlattr>.name", args["program"].as<string>());
	} else {
		programTree = experiment.get_child("Program");
	}
	programs = initPrograms(programTree, game);

	//ソルバー
	prop::ptree solverTree;
	if (args["solver"].as<string>() != "") {
		solverTree = PropertyTreeUtil::search(config, "<xmlattr>.name", args["solver"].as<string>());
	} else {
		solverTree = experiment.get_child("Solver");
	}
	solver = initSolver(solverTree, programTree);

	//出力
	prop::ptree outputTree;
	if (args["output"].as<string>() != "") {
		outputTree = PropertyTreeUtil::search(config, "<xmlattr>.name", args["output"].as<string>());
	} else {
		outputTree = experiment.get_child("Output");
	}
	initOutput(outputTree);
}

shared_ptr<Game> AiBench::initGame(prop::ptree& gameTree) {
	gameTree = PropertyTreeUtil::solveReference(config, gameTree);
	gameTree.sort();
	string gameName = gameTree.rbegin()->first;
	prop::ptree concreteGameTree = gameTree.rbegin()->second;

	shared_ptr<Game> game;
	if (gameName == "AverageAdaptor") {
		//平均化
		prop::ptree implGame = concreteGameTree.get_child("Game");
		shared_ptr<AverageAdaptor> ave = make_shared<AverageAdaptor>(initGame(implGame), 2);
		game = ave;

	} else if (gameName == "HomoAdaptor") {
		//均質化
		prop::ptree implGame = concreteGameTree.get_child("Game");
		shared_ptr<HomoAdaptor> homo = make_shared<HomoAdaptor>(initGame(implGame), 2);
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

	} else if (gameName == "Mushroom") {
		//shared_ptr<Mushroom> mush = make_shared<Mushroom>(100, 15, 200, mushroomLanguageSize, 6, time(NULL));
		//game = mush;

	} else if (gameName == "HomoMushroom") {
		//100 mush, 100(10^2)pos/mush, 200(14^2)pos/vitamin-mush
		//10 agents, 10mush/agent
		/*shared_ptr<Mushroom> mush = make_shared<Mushroom>(100, 10, 200, mushroomLanguageSize, 6, time(NULL));
		shared_ptr<HomoGame> homoMush = make_shared<HomoGame>(mush, 10, 10);
		game = homoMush;*/

	} else if (gameName == "Exchange") {
		shared_ptr<Exchange> ex = make_shared<Exchange>(concreteGameTree, time(NULL));
		game = ex;

	} else if (gameName == "Evasion") {
		shared_ptr<Evasion> eva = make_shared<Evasion>(concreteGameTree, time(NULL));
		game = eva;

	} else if (gameName == "TwoAgents") {
		shared_ptr<TwoAgents> ta = make_shared<TwoAgents>(concreteGameTree, time(NULL));
		game = ta;

	} else {
		assert(false);
	}
	game->setLogger(logger);

	cerr << "initGame: " << gameName << endl;
	cerr << "\t" << game->getProgramType() << endl;
	cerr << game->toString();
	return game;
}

vector<shared_ptr<Program>> AiBench::initPrograms(prop::ptree& programTree, shared_ptr<Game> game) {
	/*if (inputName != "") {
		ifstream ifs(inputName);
		return readPrograms(*game, gameType, programType, ifs);
	}*/

	programTree = PropertyTreeUtil::solveReference(config, programTree);
	programTree.sort();
	string programName = programTree.rbegin()->first;
	prop::ptree concreteProgramTree = programTree.rbegin()->second;

	vector<shared_ptr<Program>> programs;
	if (programName == "GraycodeMapping") {
		/*for (int i = 0; i < programNum; ++i) {
			shared_ptr<GrayCodeMapping> gray = make_shared<GrayCodeMapping>(game.getProgramType());
			gray->randomize(randomEngine);
			programs.push_back(gray);
		}*/

	} else if (programName == "ExpressionTree") {
		vector<shared_ptr<ExpressionTree>> ps = ExpressionTree::generate(game->getProgramType(), concreteProgramTree, size, randomEngine);
		for (shared_ptr<ExpressionTree> p : ps) {
			programs.push_back(p);
		}

	} else if (programName == "FeedforwardNetwork") {
		for (int i = 0; i < size; ++i) {
			programs.push_back(make_shared<FeedforwardNetwork>(game->getProgramType(), concreteProgramTree, randomEngine));
		}

	} else if (programName == "LayeredNetwork") {
		/*vector<int> nodes = {game.getProgramType().getInputLength(), 7, game.getProgramType().getOutputLength()};
		LayeredNetwork net(game.getProgramType(), nodes, [](double x){
			return NeuralLayer::sigmoid(1.0, x);
		});
		for (int i = 0; i < programNum; ++i) {
			shared_ptr<LayeredNetwork> clone = make_shared<LayeredNetwork>(net);
			clone->randomize(randomEngine);
			programs.push_back(clone);
		}*/

	} else {
		assert(false);
	}

	cerr << "initProgram: " << programName << endl;
	cerr << "\t" << programs.size() << " agents initialized" << endl;
	return programs;
}

/*vector<shared_ptr<Program>> AiBench::readPrograms(const Game& game, GameType gameType, ProgramType programType, istream& is) {
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

shared_ptr<Solver<Game>> AiBench::initSolver(prop::ptree& solverTree, prop::ptree& programTree) {
	solverTree = PropertyTreeUtil::solveReference(config, solverTree);
	solverTree.sort();
	string solverName = solverTree.rbegin()->first;
	prop::ptree concreteSolverTree = solverTree.rbegin()->second;

	shared_ptr<Solver<Game>> solver;
	if (solverName == "GeneticAlgorithm") {
		solver = initSolverImpl<GeneticAlgorithm>(concreteSolverTree, programTree);

		/*case ProgramType::GRAYCODE_MAPPING: {
			shared_ptr<GeneticAlgorithm<GrayCodeMapping>> ga = make_shared<GeneticAlgorithm<GrayCodeMapping>>(programs, time(NULL));
			ga->setScaling(GeneticAlgorithm<GrayCodeMapping>::sigmaScaling);
			ga->setCrossover(GrayCodeMapping::onePointCrossover);
			ga->setMutation(GrayCodeMapping::normalMutation);
			ga->setReinsertion(GeneticAlgorithm<GrayCodeMapping>::ElitistReinsertion);
			solver = ga;
		}*/

	} else if (solverName == "SimulatedAnnealing") {
		//TODO: broken
		/*switch (programType) {
		case ProgramType::EXPRESSION_TREE: {
			shared_ptr<SimulatedAnnealing<ExpressionTree>> sa = make_shared<SimulatedAnnealing<ExpressionTree>>(programs[0], 100, time(NULL));
			sa->setNeighbor(ExpressionTree::mutation);
			//solver = sa;
		} break;
		default:
			cout << "simulated annealing doesn't support specified program." << endl;
		}*/

	} else {
		assert(false);
	}
	solver->setLogger(logger);
	history = solver->getHistory();

	cerr << "initSolver: " << solverName << endl;
	return solver;
}

void AiBench::initOutput(boost::property_tree::ptree& outputTree) {
	typedef lg::sinks::synchronous_sink<lg::sinks::text_ostream_backend> text_sink;
	outputTree = PropertyTreeUtil::solveReference(config, outputTree);

	for (const prop::ptree::value_type& kvp : outputTree) {
		if (kvp.first == "<xmlattr>") {
			continue;
		}
		assert(kvp.first == "Logger");
		const prop::ptree& logger = kvp.second;

		string target = logger.get<string>("Target");
		string series = logger.get<string>("Series");
		string filename = logger.get<string>("Filename");

		if (target == "Relation") {
			relationFile = filename;

		} else if (target == "Evaluation") {
			pair<int, int> range = boost::lexical_cast<pair<int, int>>(series);

			//シンクの作成
			for (int i = range.first; i < range.second; ++i) {
				boost::shared_ptr<text_sink> sink = boost::make_shared<text_sink>();
				sink->locked_backend()->add_stream(boost::make_shared<ofstream>("result\\" + to_string(i) + filename));
				sink->set_filter(lg::expressions::is_in_range<int>("Evaluation", i, i + 1));
				lg::core::get()->add_sink(sink);
			}
		} else if (target == "Validation") {
			pair<int, int> range = boost::lexical_cast<pair<int, int>>(series);

			//シンクの作成
			for (int i = range.first; i < range.second; ++i) {
				boost::shared_ptr<text_sink> sink = boost::make_shared<text_sink>();
				sink->locked_backend()->add_stream(boost::make_shared<ofstream>("result\\" + to_string(i) + filename));
				sink->set_filter(lg::expressions::is_in_range<int>("Validation", i, i + 1));
				lg::core::get()->add_sink(sink);
			}
		} else if (target == "Solution") {
			pair<int, int> range = boost::lexical_cast<pair<int, int>>(series);

			//シンクの作成
			for (int i = range.first; i < range.second; ++i) {
				boost::shared_ptr<text_sink> sink = boost::make_shared<text_sink>();
				sink->locked_backend()->add_stream(boost::make_shared<ofstream>("result\\" + to_string(i) + filename));
				sink->set_filter(lg::expressions::is_in_range<int>("Solution", i, i + 1));
				lg::core::get()->add_sink(sink);
			}
		} else if (target == "Program"){
			pair<int, int> range = boost::lexical_cast<pair<int, int>>(series);

			//シンクの作成
			for (int i = range.first; i < range.second; ++i) {
				boost::shared_ptr<text_sink> sink = boost::make_shared<text_sink>();
				sink->locked_backend()->add_stream(boost::make_shared<ofstream>("result\\" + to_string(i) + filename));
				sink->set_filter(lg::expressions::is_in_range<int>("Program", i, i + 1));
				lg::core::get()->add_sink(sink);
			}
		} else {
			assert(false);
		}
	}
}

void AiBench::validate(std::vector<std::shared_ptr<Solution>>& solutions) {
	cerr << "run\tmean\tfitness" << endl;
	lg::attributes::mutable_constant<int> validationAttr(0);
	game->getLogger()->add_attribute("Validation", validationAttr);

	vector<double> mean(game->getProgramSize().first);
	for (int i = 0; i < validationTimes; ++i) {
		//プログラムの選択
		vector<Program*> best(game->getProgramSize().first);
		for (unsigned int k = 0; k < best.size(); ++k) {
			best[k] = &*(solutions[k]->getProgram());
		}

		//評価の実行
		validationAttr.set(i);
		vector<double> fitness = game->evaluate(best);
		cerr << i << "\t" << accumulate(fitness.begin(), fitness.end(), 0.0) / (double)fitness.size() << "\t" << fitness << endl;

		//平均の計算
		for (unsigned int k = 0; k < fitness.size(); ++k) {
			mean[k] += fitness[k];
		}
	}

	//平均の計算
	for (unsigned int i = 0; i < mean.size(); ++i) {
		mean[i] /= validationTimes;
	}
	cout << "(mean)\t" << accumulate(mean.begin(), mean.end(), 0.0) / (double)mean.size() << "\t" << mean << endl;
}
