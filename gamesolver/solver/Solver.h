/*
 * Solver.h
 *
 *  Created on: 2013/05/13
 *      Author: kryozahiro
 */

#ifndef SOLVER_H_
#define SOLVER_H_

#include "cpputil/reflection/Reflection.h"

#include "Evaluator.h"
#include "TerminationCriteria.h"

#define GAMESOLVER_SOLVER_MODULE(Class)\
	CPPUTIL_AT_START(Class) {\
		auto global = cpputil::Reflection::get_mutable_instance().getGlobal();\
		auto theClass = global->addScope(cpputil::Scope::CLASS, #Class);\
		theClass->addFunction("new", std::function<SolverBase*(boost::property_tree::ptree&, const std::vector<std::shared_ptr<Solution>>&)>([](boost::property_tree::ptree& solverTree, const std::vector<std::shared_ptr<Solution>>& inits) {\
			return new Class(solverTree, inits);\
		}));\
	}

class SolverBase {
public:
	virtual ~SolverBase() = default;

	/// 与えられた評価回数に到達するまで最適化する
	virtual std::vector<std::shared_ptr<Solution>> solve(Game& game, TerminationCriteria& termination, int historySize, std::pair<int, int> loggerRange) = 0;

	/// 解の履歴を取得する
	virtual std::shared_ptr<SolutionHistory> getHistory() = 0;

};

template <class GameCategory>
class Solver : public SolverBase {
public:
	virtual ~Solver() = default;

	/// 与えられた評価回数に到達するまで最適化する
	virtual std::vector<std::shared_ptr<Solution>> solve(Game& game, TerminationCriteria& termination, int historySize, std::pair<int, int> loggerRange) override final;

	/// 解の履歴を取得する
	virtual std::shared_ptr<SolutionHistory> getHistory() override final;

protected:
	virtual std::vector<std::shared_ptr<Solution>> solveImpl(Evaluator& evaluator, TerminationCriteria& termination) = 0;

private:
	/// 評価器
	std::shared_ptr<Evaluator> evaluator;

	/// 解の履歴
	std::shared_ptr<SolutionHistory> history;
};

template <class GameCategory>
std::vector<std::shared_ptr<Solution>> Solver<GameCategory>::solve(Game& game, TerminationCriteria& termination, int historySize, std::pair<int, int> loggerRange) {
	history = std::make_shared<SolutionHistory>(historySize);
	std::shared_ptr<Game> sharedGame(&game, [](Game*){});
	evaluator = std::make_shared<Evaluator>(sharedGame, "Evaluation", loggerRange);
	history->start();
	return solveImpl(*evaluator, termination);
}

template <class GameCategory>
std::shared_ptr<SolutionHistory> Solver<GameCategory>::getHistory() {
	return history;
}

#endif /* SOLVER_H_ */
