/*
 * GeneratorStage.cpp
 *
 *  Created on: 2015/07/22
 *      Author: kryozahiro
 */

#include "GeneratorStage.h"

#include <cassert>
#include "cpputil/reflection/Reflection.h"
#include "cpputil/PropertyTreeUtil.h"
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


	/*if (programName == "GraycodeMapping") {
		for (int i = 0; i < programNum; ++i) {
			shared_ptr<GrayCodeMapping> gray = make_shared<GrayCodeMapping>(game.getProgramType());
			gray->randomize(randomEngine);
			programs.push_back(gray);
		}
	}*/

	/*} else if (programName == "ExpressionTree") {
		vector<shared_ptr<ExpressionTree>> ps = ExpressionTree::generate(game->getProgramType(), concreteProgramTree, size, randomEngine);
		for (shared_ptr<ExpressionTree> p : ps) {
			programs.push_back(p);
		}

	} else if (programName == "InstructionSequence") {
		for (int i = 0; i < size; ++i) {
			programs.push_back(make_shared<InstructionSequence>(game->getProgramType(), concreteProgramTree, size, randomEngine));
		}

	} else if (programName == "FeedforwardNetwork") {*/


	vector<shared_ptr<Program>> programs;
	auto global = Reflection::get_mutable_instance().getGlobal();
	auto theClass = global->getScope(programName);
	auto genOp = theClass->getFunction<std::vector<std::shared_ptr<Program>>(const ProgramType&, const boost::property_tree::ptree&, int size, std::mt19937_64&)>("generate");
	programs = vector<shared_ptr<Program>>(genOp(game->getProgramType(), concreteProgramTree, size, randomEngine));

	cerr << "Program: " << programName << endl;
	this->programName = programName;
	return programs;
}

std::string GeneratorStage::getProgramName() const {
	return programName;
}
