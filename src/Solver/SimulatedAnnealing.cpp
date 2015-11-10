/*
 * SimulatedAnnealing.cpp
 *
 *  Created on: 2015/11/11
 *      Author: kryozahiro
 */

#include <iostream>
#include "NeighborhoodOperable.h"
#include "SimulatedAnnealing.h"
using namespace std;
using namespace cpputil;

SimulatedAnnealing::SimulatedAnnealing(const std::shared_ptr<Solution>& init, double initTemp, int seed) : randomEngine(seed) {
	this->init = init;
	this->initTemp = initTemp;
}

SimulatedAnnealing::SimulatedAnnealing(boost::property_tree::ptree& node, const std::vector<std::shared_ptr<Solution>>& inits) :
		init(inits[0]) {
	//乱数の初期化
	std::string seed = node.get<std::string>("Seed");
	if (seed == "Random") {
		randomEngine = std::mt19937_64(time(NULL));
	} else {
		randomEngine = std::mt19937_64(atoi(seed.c_str()));
	}
	this->initTemp = node.get<double>("InitialTemperature");
	setNeighborhood(node.get<std::string>("Neighborhood"), node.get<int>("NeighborhoodSize"));
}

void SimulatedAnnealing::setNeighborhood(const std::string& neighborhood, int size) {
	this->neighborhood = neighborhood;
	this->neighborhoodSize = size;
}

std::vector<std::shared_ptr<Solution>> SimulatedAnnealing::solveImpl(Evaluator& evaluator, TerminationCriteria& termination) {
	//初期解を評価
	double temp = initTemp;
	std::shared_ptr<Solution> now = init;
	std::vector<std::shared_ptr<Solution>> nows(1, now);
	Evaluator::problemEvaluation(evaluator, nows, *getHistory(), randomEngine);
	getHistory()->addGeneration(nows);

	while (!termination.meets(evaluator.getEvaluationCount(), *getHistory())) {
		//近傍を生成して評価
		std::shared_ptr<NeighborhoodOperableBase> program = std::dynamic_pointer_cast<NeighborhoodOperableBase>(now->getProgram());
		std::vector<std::shared_ptr<NeighborhoodOperableBase>> neighbors = program->neighborhood(neighborhood, neighborhoodSize, randomEngine);
		std::vector<std::shared_ptr<Solution>> children;
		for (std::shared_ptr<NeighborhoodOperableBase> neighbor : neighbors) {
			children.push_back(make_shared<Solution>(std::dynamic_pointer_cast<Program>(neighbor)));
			children.back()->setGeneration(now->getGeneration() + 1);
			now->addChild(children.back());
		}
		Evaluator::problemEvaluation(evaluator, children, *getHistory(), randomEngine);
		getHistory()->addGeneration(children);

		//最も良い近傍を選択
		std::sort(children.begin(), children.end(), [](std::shared_ptr<Solution> lhs, std::shared_ptr<Solution> rhs){
			return lhs->getFitness() < rhs->getFitness();
		});
		std::shared_ptr<Solution> child = children[0];

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
