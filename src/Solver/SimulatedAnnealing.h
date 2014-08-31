/*
 * SimulatedAnnealing.h
 *
 *  Created on: 2014/02/02
 *      Author: kryozahiro
 */

#ifndef SIMULATEDANNEALING_H_
#define SIMULATEDANNEALING_H_

#include <random>
#include "Solver.h"

//TODO: class ProblemSolver ?

template <class ConcreteProgram>
class SimulatedAnnealing : public Solver<Problem> {
public:
	using Neighbor = std::function<void(ConcreteProgram& program, std::mt19937_64& randomEngine)>;

	SimulatedAnnealing(const std::shared_ptr<Program>& init, double initTemp, int seed) : randomEngine(seed) {
		this->init = std::make_shared<Solution>(init);
		this->init->setGeneration(0);
		this->initTemp = initTemp;
		this->temp = initTemp;
	}
	virtual ~SimulatedAnnealing() = default;

	void setNeighbor(Neighbor neighbor) {
		this->neighbor = neighbor;
	}

protected:
	virtual std::vector<std::shared_ptr<Solution>> solveImpl(Problem& problem, int evaluateNum) override {
		//初期解を評価
		temp = initTemp;
		std::shared_ptr<Solution> now = std::make_shared<Solution>(*init);
		now->setFitness(problem.evaluate(*now->getProgram()));
		getHistory()->addSolution(now);

		for (int i = 0; i < evaluateNum; ++i) {
			//近傍を生成して評価
			std::shared_ptr<Solution> child = now->createChild();
			neighbor(*std::dynamic_pointer_cast<ConcreteProgram>(child->getProgram()), randomEngine);
			child->setFitness(problem.evaluate(*child->getProgram()));
			getHistory()->addSolution(child);

			//取り入れるか決定
			std::uniform_real_distribution<double> dist(0, 1);
			double delta = child->getFitness() - now->getFitness();
			if (delta < 0 or exp(-delta / temp) > dist(randomEngine)) {
				now = child;
			}
			temp *= 0.95;
		}
		return std::vector<std::shared_ptr<Solution>>(1, now);
	}

private:
	std::shared_ptr<Solution> init;
	double initTemp;
	double temp;
	std::mt19937_64 randomEngine;
	Neighbor neighbor = [](ConcreteProgram& program, std::mt19937_64& randomEngine){};
};

#endif /* SIMULATEDANNEALING_H_ */
