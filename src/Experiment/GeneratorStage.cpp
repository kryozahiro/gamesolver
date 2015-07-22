/*
 * GeneratorStage.cpp
 *
 *  Created on: 2015/07/22
 *      Author: kryozahiro
 */

#include <cassert>
#include "CppUtil/PropertyTreeUtil.h"
#include "../Program/IoMapping/GrayCodeMapping.h"
#include "../Program/ExpressionTree/ExpressionTree.h"
#include "../Program/InstructionSequence/InstructionSequence.h"
#include "../Program/NeuralNetwork/FeedforwardNetwork.h"
#include "GeneratorStage.h"
using namespace std;
using namespace cpputil;
namespace pt = boost::property_tree;

GeneratorStage::GeneratorStage(const boost::property_tree::ptree& config, const boost::property_tree::ptree& generatorStageTree, std::shared_ptr<Game>& game) : game(game) {
	programTree = generatorStageTree.get_child("Program");
	programTree = cpputil::solveReference(config, programTree);
	programTree.sort();
	programName = programTree.rbegin()->first;

	//プログラムのサイズ
	/*if (args["agents"].as<int>() != 0) {
		size = args["agents"].as<int>();
	}*/
	size = generatorStageTree.get<int>("Size");
}

void GeneratorStage::operator()(std::vector<std::shared_ptr<Solution>>& solutions, std::mt19937_64& randomEngine) {
	vector<shared_ptr<Program>> inits = initPrograms(randomEngine);

	cerr << "size = " << size << endl;
	cerr << endl;

	//初期解の設定
	solutions.clear();
	for (unsigned int i = 0; i < inits.size(); ++i) {
		solutions.push_back(std::make_shared<Solution>(inits[i]));
		solutions.back()->setGeneration(0);
	}
}

vector<shared_ptr<Program>> GeneratorStage::initPrograms(std::mt19937_64& randomEngine) {
	/*if (inputName != "") {
		ifstream ifs(inputName);
		return readPrograms(*game, gameType, programType, ifs);
	}*/

	pt::ptree concreteProgramTree = programTree.rbegin()->second;

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

	} else if (programName == "InstructionSequence") {
		for (int i = 0; i < size; ++i) {
			programs.push_back(make_shared<InstructionSequence>(game->getProgramType(), concreteProgramTree, randomEngine));
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
	assert(programs.size() == (unsigned int)size);

	cerr << "Program: " << programName << endl;
	return programs;
}

std::string GeneratorStage::getProgramName() const {
	return programName;
}
