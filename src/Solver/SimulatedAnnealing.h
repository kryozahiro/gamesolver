/*
 * SimulatedAnnealing.h
 *
 *  Created on: 2014/02/02
 *      Author: kryozahiro
 */

#ifndef SIMULATEDANNEALING_H_
#define SIMULATEDANNEALING_H_

#include <random>
#include <boost/property_tree/ptree.hpp>
#include "ProblemSolver.h"

class SimulatedAnnealing : public ProblemSolver {
public:
	SimulatedAnnealing(const std::shared_ptr<Solution>& init, double initTemp, int seed);
	SimulatedAnnealing(boost::property_tree::ptree& node, const std::vector<std::shared_ptr<Solution>>& inits);
	virtual ~SimulatedAnnealing() = default;

	void setNeighbor(const std::string& neighbor);

protected:
	virtual std::vector<std::shared_ptr<Solution>> solveImpl(Evaluator& evaluator, TerminationCriteria& termination) override;

private:
	std::shared_ptr<Solution> init;
	std::mt19937_64 randomEngine;

	double initTemp;
	std::string neighbor;
};

GAMESOLVER_SOLVER_MODULE(SimulatedAnnealing);

#endif /* SIMULATEDANNEALING_H_ */
