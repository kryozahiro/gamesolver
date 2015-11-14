/*
 * ProblemSolver.h
 *
 *  Created on: 2015/11/08
 *      Author: kryozahiro
 */

#ifndef GAMESOLVER_SOLVER_PROBLEMSOLVER_H_
#define GAMESOLVER_SOLVER_PROBLEMSOLVER_H_

#include "../problem/Problem.h"
#include "Solver.h"

class ProblemSolver : public Solver<Problem> {
private:
	//virtual std::vector<std::shared_ptr<Solution>> solveImpl(Evaluator& evaluator, TerminationCriteria& termination) override final;
};

#endif /* GAMESOLVER_SOLVER_PROBLEMSOLVER_H_ */
