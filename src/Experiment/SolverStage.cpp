/*
 * SolverStage.cpp
 *
 *  Created on: 2015/07/22
 *      Author: kryozahiro
 */

#include <cassert>
#include "CppUtil/PropertyTreeUtil.h"
#include "../Solver/GeneticAlgorithm.h"
#include "../Solver/SimulatedAnnealing.h"
#include "SolverStage.h"
using namespace std;
using namespace cpputil;
namespace pt = boost::property_tree;

SolverStage::SolverStage(const boost::property_tree::ptree& config, const boost::property_tree::ptree& solverStageTree, std::shared_ptr<Game>& game) : game(game) {
	solverTree = solverStageTree.get_child("Solver");
	solverTree = cpputil::solveReference(config, solverTree);
	solverTree.sort();
	solverName = solverTree.rbegin()->first;

	//終了条件
	pt::ptree terminationTree = solverStageTree.get_child("Termination");
	termination = make_shared<TerminationCriteria>(terminationTree);

	//解の履歴
	/*if (args["history"].as<int>() != 0) {
		historySize = args["history"].as<int>();
	}*/
	historySize = solverStageTree.get<int>("History");
}

void SolverStage::operator()(std::vector<std::shared_ptr<Solution>>& solutions, std::mt19937_64& randomEngine) {
	solver = initSolver(solutions);

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

void SolverStage::set(std::shared_ptr<GeneratorStage>& generatorStage, std::pair<int, int> evaluationLoggerRange) {
	this->generatorStage = generatorStage;
	this->evaluationLoggerRange = evaluationLoggerRange;
}

const std::shared_ptr<SolutionHistory> SolverStage::getHistory() const {
	return solver->getHistory();
}

shared_ptr<Solver<Game>> SolverStage::initSolver(std::vector<std::shared_ptr<Solution>>& solutions) {
	pt::ptree concreteSolverTree = solverTree.rbegin()->second;

	shared_ptr<Solver<Game>> solver;
	if (solverName == "GeneticAlgorithm") {
		solver = initSolverImpl<GeneticAlgorithm>(concreteSolverTree, solutions);

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
	cerr << "Solver: " << solverName << endl;
	return solver;
}
