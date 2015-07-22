/*
 * SolverStage.h
 *
 *  Created on: 2015/07/22
 *      Author: kryozahiro
 */

#ifndef SRC_EXPERIMENT_SOLVERSTAGE_H_
#define SRC_EXPERIMENT_SOLVERSTAGE_H_

#include "../Program/IoMapping/GrayCodeMapping.h"
#include "../Program/ExpressionTree/ExpressionTree.h"
#include "../Program/InstructionSequence/InstructionSequence.h"
#include "../Program/NeuralNetwork/FeedforwardNetwork.h"
#include "../Solver/Solver.h"
#include "GeneratorStage.h"
#include "Stage.h"

class SolverStage : public Stage {
public:
	SolverStage(const boost::property_tree::ptree& config, const boost::property_tree::ptree& solverStageTree, std::shared_ptr<Game>& game);
	virtual ~SolverStage() = default;

	//Stageの実装
	virtual void operator()(std::vector<std::shared_ptr<Solution>>& solutions, std::mt19937_64& randomEngine);

	void set(std::shared_ptr<GeneratorStage>& generatorStage, std::pair<int, int> evaluationLoggerRange);

	const std::shared_ptr<SolutionHistory> getHistory() const;

private:
	//ソルバの読み込み
	std::shared_ptr<Solver<Game>> initSolver(std::vector<std::shared_ptr<Solution>>& solutions);

	template <template <class ConcreteProgram> class ConcreteSolver>
	std::shared_ptr<Solver<Game>> initSolverImpl(boost::property_tree::ptree& solverTree, std::vector<std::shared_ptr<Solution>>& solutions) {
		std::vector<std::shared_ptr<Program>> programs;
		for (std::shared_ptr<Solution>& solution : solutions) {
			programs.push_back(solution->getProgram());
		}

		std::shared_ptr<Solver<Game>> solver;
		std::string programName = generatorStage->getProgramName();
		if (programName == "GrayCodeMapping") {
			//solver = ConcreteSolver<GrayCodeMapping>(solverTree, programs, history);
		} else if (programName == "ExpressionTree") {
			solver = std::make_shared<ConcreteSolver<ExpressionTree>>(solverTree, programs);
		} else if (programName == "InstructionSequence") {
			solver = std::make_shared<ConcreteSolver<InstructionSequence>>(solverTree, programs);
		} else if (programName == "FeedforwardNetwork") {
			solver = std::make_shared<ConcreteSolver<FeedforwardNetwork>>(solverTree, programs);
		} else {
			assert(false);
		}
		return solver;
	}

	std::shared_ptr<Game> game;
	boost::property_tree::ptree solverTree;
	std::string solverName;
	std::shared_ptr<TerminationCriteria> termination;
	int historySize;

	std::shared_ptr<GeneratorStage> generatorStage;
	std::pair<int, int> evaluationLoggerRange;

	std::shared_ptr<Solver<Game>> solver;
};

#endif /* SRC_EXPERIMENT_SOLVERSTAGE_H_ */
