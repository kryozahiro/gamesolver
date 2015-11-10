/*
 * SolverStage.h
 *
 *  Created on: 2015/07/22
 *      Author: kryozahiro
 */

#ifndef SRC_EXPERIMENT_SOLVERSTAGE_H_
#define SRC_EXPERIMENT_SOLVERSTAGE_H_

#include "../Solver/Solver.h"
#include "GeneratorStage.h"
#include "Stage.h"

class SolverStage : public Stage {
public:
	SolverStage(const boost::property_tree::ptree& config, const boost::property_tree::ptree& solverStageTree, std::shared_ptr<Game>& game, std::string programName, std::pair<int, int> evaluationLoggerRange);
	virtual ~SolverStage() = default;

	//Stageの実装
	virtual void operator()(std::vector<std::shared_ptr<Solution>>& solutions, std::mt19937_64& randomEngine);

	//解の履歴を取得する
	const std::shared_ptr<SolutionHistory> getHistory() const;

private:
	//ソルバの読み込み
	std::shared_ptr<SolverBase> createSolver(boost::property_tree::ptree& solverTree, std::vector<std::shared_ptr<Solution>>& solutions);

	/*template <template <class ConcreteProgram> class ConcreteSolver>
	std::shared_ptr<Solver<Game>> createSolverImpl(boost::property_tree::ptree& solverTree, std::vector<std::shared_ptr<Solution>>& solutions) {
		std::shared_ptr<Solver<Game>> solver;
		if (programName == "GrayCodeMapping") {
			//solver = ConcreteSolver<GrayCodeMapping>(solverTree, programs, history);
		} else if (programName == "ExpressionTree") {
			solver = std::make_shared<ConcreteSolver<ExpressionTree>>(solverTree, solutions);
		} else if (programName == "InstructionSequence") {
			solver = std::make_shared<ConcreteSolver<InstructionSequence>>(solverTree, solutions);
		} else if (programName == "FeedforwardNetwork") {
			solver = std::make_shared<ConcreteSolver<FeedforwardNetwork>>(solverTree, solutions);
		} else {
			assert(false);
		}
		return solver;
	}*/

	//<Config>ノード
	const boost::property_tree::ptree config;

	//<SolverStage>ノード
	const boost::property_tree::ptree solverStageTree;

	//使用するGame
	std::shared_ptr<Game> game;

	//使用するProgram
	std::string programName;

	//ロギングの設定
	std::pair<int, int> evaluationLoggerRange;

	//終了条件
	std::shared_ptr<TerminationCriteria> termination;
	int historySize;

	//ソルバ
	std::string solverName;
	std::shared_ptr<SolverBase> solver;
};

#endif /* SRC_EXPERIMENT_SOLVERSTAGE_H_ */
