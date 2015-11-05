/*
 * GeneratorStage.cpp
 *
 *  Created on: 2015/07/22
 *      Author: kryozahiro
 */

#include <cassert>
#include "cpputil/PropertyTreeUtil.h"
#include "../Program/IoMapping/GrayCodeMapping.h"
#include "../Program/ExpressionTree/ExpressionTree.h"
#include "../Program/InstructionSequence/InstructionSequence.h"
#include "../Program/NeuralNetwork/FeedforwardNetwork.h"
#include "GeneratorStage.h"
using namespace std;
using namespace cpputil;
namespace pt = boost::property_tree;

GeneratorStage::GeneratorStage(const boost::property_tree::ptree& config, const boost::property_tree::ptree& generatorStageTree, std::shared_ptr<Game>& game) :
		config(config), generatorStageTree(generatorStageTree), game(game) {
	size = generatorStageTree.get<int>("Size");
}

void GeneratorStage::operator()(std::vector<std::shared_ptr<Solution>>& solutions, std::mt19937_64& randomEngine) {
	pt::ptree programTree = generatorStageTree.get_child("Program");
	vector<shared_ptr<Program>> inits = createPrograms(programTree, randomEngine);
	assert(inits.size() == (unsigned int)size);

	cerr << "size = " << size << endl;
	cerr << endl;

	//初期解の設定
	solutions.clear();
	for (unsigned int i = 0; i < inits.size(); ++i) {
		solutions.push_back(std::make_shared<Solution>(inits[i]));
		solutions.back()->setGeneration(0);
	}
}

vector<shared_ptr<Program>> GeneratorStage::createPrograms(boost::property_tree::ptree& programTree, std::mt19937_64& randomEngine) {
	/*if (inputName != "") {
		ifstream ifs(inputName);
		return readPrograms(*game, gameType, programType, ifs);
	}*/

	programTree = cpputil::solveReference(config, programTree);
	programTree.sort();
	string programName = programTree.rbegin()->first;
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

	cerr << "Program: " << programName << endl;
	this->programName = programName;
	return programs;
}

std::string GeneratorStage::getProgramName() const {
	return programName;
}
