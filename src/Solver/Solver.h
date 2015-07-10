/*
 * Solver.h
 *
 *  Created on: 2013/05/13
 *      Author: kryozahiro
 */

#ifndef SOLVER_H_
#define SOLVER_H_

#include "Evaluator.h"
#include "TerminationCriteria.h"

template <class GameCategory>
class Solver {
public:
	virtual ~Solver() = default;

	//与えられた評価回数に到達するまで最適化する
	std::vector<std::shared_ptr<Solution>> solve(GameCategory& game, TerminationCriteria& termination, int historySize, std::pair<int, int> loggerRange);

	//解の履歴を取得する
	std::shared_ptr<SolutionHistory> getHistory();

protected:
	virtual std::vector<std::shared_ptr<Solution>> solveImpl(Evaluator& evaluator, TerminationCriteria& termination) = 0;

private:
	//評価器
	std::shared_ptr<Evaluator> evaluator;

	//解の履歴
	std::shared_ptr<SolutionHistory> history;
};

template <class GameCategory>
std::vector<std::shared_ptr<Solution>> Solver<GameCategory>::solve(GameCategory& game, TerminationCriteria& termination, int historySize, std::pair<int, int> loggerRange) {
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
