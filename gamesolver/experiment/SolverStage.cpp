/*
 * SolverStage.cpp
 *
 *  Created on: 2015/07/22
 *      Author: kryozahiro
 */

#include "SolverStage.h"

#include <cassert>
#include "cpputil/PropertyTreeUtil.h"
#include "cpputil/reflection/Reflection.h"
using namespace std;
using namespace cpputil;
namespace pt = boost::property_tree;

SolverStage::SolverStage(const boost::property_tree::ptree& config, const boost::property_tree::ptree& solverStageTree, std::shared_ptr<Game>& game, std::string programName, std::pair<int, int> evaluationLoggerRange) :
		config(config), solverStageTree(solverStageTree), game(game), programName(programName), evaluationLoggerRange(evaluationLoggerRange) {
	//終了条件
	pt::ptree terminationTree = solverStageTree.get_child("Termination");
	termination = make_shared<TerminationCriteria>(terminationTree);

	//解の履歴
	historySize = solverStageTree.get<int>("History");
}

void SolverStage::operator()(std::vector<std::shared_ptr<Solution>>& solutions, std::mt19937_64& randomEngine) {
	pt::ptree solverTree = solverStageTree.get_child("Solver");
	solver = createSolver(solverTree, solutions);

	//探索の実行
	cerr << "termination = " << "(impl)" << endl;
	cerr << "history = " << historySize << endl;
	cerr << "gen\tbest\tworst\tmean\tsd" << endl;
	solutions = solver->solve(*game, *termination, historySize, evaluationLoggerRange);
	cerr << solver->getHistory()->getElapsedTime() << " sec elapsed" << endl;
	cerr << endl;

	cerr << "Solution:" << endl;
	cerr << solutions[0]->toString();
	cerr << endl;
}

const std::shared_ptr<SolutionHistory> SolverStage::getHistory() const {
	return solver->getHistory();
}

shared_ptr<SolverBase> SolverStage::createSolver(boost::property_tree::ptree& solverTree, std::vector<std::shared_ptr<Solution>>& solutions) {
	solverTree = cpputil::solveReference(config, solverTree);
	solverTree.sort();
	string solverName = solverTree.rbegin()->first;
	pt::ptree concreteSolverTree = solverTree.rbegin()->second;

	shared_ptr<SolverBase> solver;
	auto global = Reflection::get_mutable_instance().getGlobal();
	auto theClass = global->getScope(solverName);
	auto newOp = theClass->getFunction<SolverBase*(boost::property_tree::ptree&, const std::vector<std::shared_ptr<Solution>>&)>("new");
	solver = std::shared_ptr<SolverBase>(newOp(concreteSolverTree, solutions));

	cerr << "Solver: " << solverName << endl;
	this->solverName = solverName;
	return solver;
}
