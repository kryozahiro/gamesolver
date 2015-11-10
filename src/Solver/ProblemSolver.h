/*
 * ProblemSolver.h
 *
 *  Created on: 2015/11/08
 *      Author: kryozahiro
 */

#ifndef SRC_SOLVER_PROBLEMSOLVER_H_
#define SRC_SOLVER_PROBLEMSOLVER_H_

#include "Solver.h"
#include "../Problem/Problem.h"

class ProblemSolver : public Solver<Problem> {
private:
	//virtual std::vector<std::shared_ptr<Solution>> solveImpl(Evaluator& evaluator, TerminationCriteria& termination) override final;
};

#endif /* SRC_SOLVER_PROBLEMSOLVER_H_ */
