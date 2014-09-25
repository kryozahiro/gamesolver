/*
 * Solver.h
 *
 *  Created on: 2013/05/13
 *      Author: kryozahiro
 */

#ifndef SOLVER_H_
#define SOLVER_H_

#include "Evaluator.h"

template <class GameCategory>
class Solver {
public:
	Solver() : history(std::make_shared<SolutionHistory>()) {}
	virtual ~Solver() = default;

	//与えられた評価回数に到達するまで最適化する
	std::vector<std::shared_ptr<Solution>> solve(GameCategory& game, int evaluateNum, std::pair<int, int> loggerRange) {
		evaluator = std::make_shared<Evaluator>(game, "Evaluation", loggerRange);
		history->start();
		return solveImpl(*evaluator, evaluateNum);
	}

	//解の履歴を取得する
	std::shared_ptr<SolutionHistory> getHistory() {
		return history;
	}

protected:
	virtual std::vector<std::shared_ptr<Solution>> solveImpl(Evaluator& evaluator, int evaluateNum) = 0;

private:
	//評価器
	std::shared_ptr<Evaluator> evaluator;

	//解の履歴
	std::shared_ptr<SolutionHistory> history;
};

#endif /* SOLVER_H_ */
