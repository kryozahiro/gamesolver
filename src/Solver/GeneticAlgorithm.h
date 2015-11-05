/*
 * GeneticAlgorithm.h
 *
 *  Created on: 2013/05/13
 *      Author: kryozahiro
 */

#ifndef GENETICALGORITHM_H_
#define GENETICALGORITHM_H_

#include <algorithm>
#include <random>
#include <boost/property_tree/ptree.hpp>
#include "cpputil/AlgorithmUtil.h"
#include "cpputil/MathUtil.h"
#include "Solver.h"
#include "GeneticOperable.h"

#include <iostream>

template <class ConcreteGeneticOperableProgram>
class GeneticAlgorithm : public Solver<Game> {
public:
	using Scaling = std::function<void(std::vector<double>& weights)>;
	using Selection = std::function<int(std::vector<double>& weights, std::mt19937_64& randomEngine)>;
	using Reinsertion = std::function<void(std::vector<std::shared_ptr<Solution>>& population, std::vector<std::shared_ptr<Solution>>& offspring)>;

	GeneticAlgorithm(const std::vector<std::shared_ptr<Solution>>& inits, int seed);
	GeneticAlgorithm(boost::property_tree::ptree& node, const std::vector<std::shared_ptr<Solution>>& inits);
	virtual ~GeneticAlgorithm() = default;

	//操作の設定
	void setScaling(Scaling scaling);
	void setReproductionRate(double reproductionRate);
	void setElitismRate(double elitismRate);
	void setSelection(Selection selection);
	void setCrossover(std::string crossoverName);
	void setMutation(std::string mutationName);
	void setEvaluation(Evaluator::Evaluation evaluation);
	void setReinsertion(Reinsertion reinsertion);

	//スケーリング
	static void linearScaling(std::vector<double>& weights);
	static void sigmaScaling(std::vector<double>& weights);

	//ルーレット選択
	static int rouletteSelection(std::vector<double> weights, std::mt19937_64& randomEngine);
	//トーナメント選択
	static int tournamentSelection(std::vector<double> weights, double tournamentRate, std::mt19937_64& randomEngine);

	//再挿入
	static void pureReinsertion(std::vector<std::shared_ptr<Solution>>& population, std::vector<std::shared_ptr<Solution>>& offspring);
	//TODO: uniform reinsertion
	static void elitistReinsertion(std::vector<std::shared_ptr<Solution>>& population, std::vector<std::shared_ptr<Solution>>& offspring);
	//TODO: fitness-based reinsertion

protected:
	virtual std::vector<std::shared_ptr<Solution>> solveImpl(Evaluator& evaluator, TerminationCriteria& termination) override;

private:
	//世代を進める
	//評価した回数を返す
	void advanceGeneration(Evaluator& evaluator, std::vector<std::shared_ptr<Solution>>& genes);

	//初期解
	std::vector<std::shared_ptr<Solution>> inits;
	std::mt19937_64 randomEngine;

	//遺伝的操作
	Scaling scaling = [](std::vector<double>& weights){};
	double reproductionRate = 1.0;
	double elitismRate = 0.0;
	Selection selection = rouletteSelection;
	std::string crossover;
	std::string mutation;
	Evaluator::Evaluation evaluation;
	Reinsertion reinsertion = pureReinsertion;
};

template <class ConcreteGeneticOperableProgram>
GeneticAlgorithm<ConcreteGeneticOperableProgram>::GeneticAlgorithm(const std::vector<std::shared_ptr<Solution>>& inits, int seed)
: inits(inits), randomEngine(seed) {
}

template <class ConcreteGeneticOperableProgram>
GeneticAlgorithm<ConcreteGeneticOperableProgram>::GeneticAlgorithm(boost::property_tree::ptree& node, const std::vector<std::shared_ptr<Solution>>& inits)
: inits(inits) {
	//乱数の初期化
	std::string seed = node.get<std::string>("Seed");
	if (seed == "Random") {
		randomEngine = std::mt19937_64(time(NULL));
	} else {
		randomEngine = std::mt19937_64(atoi(seed.c_str()));
	}

	//Scalingの設定
	std::string scaling = node.get<std::string>("Scaling");
	if (scaling == "LinearScaling") {
		setScaling(linearScaling);
	} else if (scaling == "SigmaScaling") {
		setScaling(sigmaScaling);
	} else {
		assert(false);
	}

	//ReproductionRateの設定
	double reproductionRate = node.get<double>("ReproductionRate");
	setReproductionRate(reproductionRate);

	//ElitismRateの設定
	double elitismRate = node.get<double>("ElitismRate");
	setElitismRate(elitismRate);

	//Selectionの設定
	std::string selection = node.get<std::string>("Selection");
	if (selection == "RouletteSelection") {
		setSelection(rouletteSelection);
	} else if (selection == "TournamentSelection") {
		double tournamentRate = node.get<double>("TournamentRate");
		setSelection([=](std::vector<double>& weights, std::mt19937_64 randomEngine) {
			return tournamentSelection(weights, tournamentRate, randomEngine);
		});
	} else {
		assert(false);
	}

	//Crossoverの設定
	std::string crossoverName = node.get<std::string>("Crossover");
	setCrossover(crossoverName);

	//Mutationの設定
	std::string mutationName = node.get<std::string>("Mutation");
	setMutation(mutationName);

	//Evaluationの設定
	std::string evaluatorName = node.get<std::string>("Evaluation");
	if (evaluatorName == "ProblemEvaluation") {
		setEvaluation(Evaluator::problemEvaluation);
	} else if (evaluatorName == "AllVsAllEvaluation") {
		setEvaluation(Evaluator::allVsAllEvaluation);
	} else if (evaluatorName == "AllVsBestEvaluation") {
		setEvaluation(Evaluator::allVsBestEvaluation);
	} else if (evaluatorName == "PartitioningEvaluation") {
		setEvaluation(Evaluator::partitioningEvaluation);
	} else {
		assert(false);
	}

	//Reinsertionの設定
	std::string reinsertionName = node.get<std::string>("Reinsertion");
	if (reinsertionName == "PureReinsertion") {
		setReinsertion(pureReinsertion);
	} else if (reinsertionName == "ElitistReinsertion") {
		setReinsertion(elitistReinsertion);
	} else {
		assert(false);
	}
}

template <class ConcreteGeneticOperableProgram>
void GeneticAlgorithm<ConcreteGeneticOperableProgram>::setScaling(Scaling scaling) {
	this->scaling = scaling;
}

template <class ConcreteGeneticOperableProgram>
void GeneticAlgorithm<ConcreteGeneticOperableProgram>::setReproductionRate(double reproductionRate) {
	this->reproductionRate = reproductionRate;
}

template <class ConcreteGeneticOperableProgram>
void GeneticAlgorithm<ConcreteGeneticOperableProgram>::setElitismRate(double elitismRate) {
	assert(0.0 <= elitismRate and elitismRate <= 1.0);
	this->elitismRate = elitismRate;
}

template <class ConcreteGeneticOperableProgram>
void GeneticAlgorithm<ConcreteGeneticOperableProgram>::setSelection(Selection selection) {
	this->selection = selection;
}

template <class ConcreteGeneticOperableProgram>
void GeneticAlgorithm<ConcreteGeneticOperableProgram>::setCrossover(std::string crossoverName) {
	this->crossover = crossoverName;
}

template <class ConcreteGeneticOperableProgram>
void GeneticAlgorithm<ConcreteGeneticOperableProgram>::setMutation(std::string mutationName) {
	this->mutation = mutationName;
}

template <class ConcreteGeneticOperableProgram>
void GeneticAlgorithm<ConcreteGeneticOperableProgram>::setEvaluation(Evaluator::Evaluation evaluation) {
	this->evaluation = evaluation;
}

template <class ConcreteGeneticOperableProgram>
void GeneticAlgorithm<ConcreteGeneticOperableProgram>::setReinsertion(Reinsertion reinsertion) {
	this->reinsertion = reinsertion;
}

template <class ConcreteGeneticOperableProgram>
void GeneticAlgorithm<ConcreteGeneticOperableProgram>::linearScaling(std::vector<double>& weights) {
	double scaling = 2;

	const double max = weights.front();
	double mean = std::accumulate(weights.begin(), weights.end(), 0.0) / weights.size();

	//変換
	if (max == mean) {
		return;
	}
	for (double& weight : weights) {
		weight = cpputil::scale(weight, mean, max, scaling);
	}
}

template <class ConcreteGeneticOperableProgram>
void GeneticAlgorithm<ConcreteGeneticOperableProgram>::sigmaScaling(std::vector<double>& weights) {
	double scaling = 2;

	double mean = std::accumulate(weights.begin(), weights.end(), 0.0) / weights.size();
	double sd = std::accumulate(weights.begin(), weights.end(), 0.0, [&](double sum, double lhs) {
		return sum + (lhs - mean) * (lhs - mean);
	}) / weights.size();
	sd = sqrt(sd);

	//変換
	for (double& weight : weights) {
		weight = weight - mean + scaling * sd;
	}
}

template <class ConcreteGeneticOperableProgram>
int GeneticAlgorithm<ConcreteGeneticOperableProgram>::rouletteSelection(std::vector<double> weights, std::mt19937_64& randomEngine) {
	double totalWeight = std::accumulate(weights.begin(), weights.end(), 0.0);
	std::uniform_real_distribution<double> dist(0, totalWeight);
	double select = dist(randomEngine);
	for (unsigned int i = 0; i < weights.size(); ++i) {
		select -= weights[i];
		if (select < 0) {
			return i;
		}
	}
	assert(false);
	return 0;
}

template <class ConcreteGeneticOperableProgram>
int GeneticAlgorithm<ConcreteGeneticOperableProgram>::tournamentSelection(std::vector<double> weights, double tournamentRate, std::mt19937_64& randomEngine) {
	int tournamentSize = weights.size() * tournamentRate;
	cpputil::partial_shuffle(weights.begin(), weights.begin() + tournamentSize, weights.end(), randomEngine);
	auto maxItr = max_element(weights.begin(), weights.begin() + tournamentSize);
	return maxItr - weights.begin();
}

template <class ConcreteGeneticOperableProgram>
void GeneticAlgorithm<ConcreteGeneticOperableProgram>::pureReinsertion(std::vector<std::shared_ptr<Solution>>& population, std::vector<std::shared_ptr<Solution>>& offspring) {
	//子孫が十分ある場合
	if (offspring.size() >= population.size()) {
		offspring.resize(population.size());
		population = offspring;
		return;
	}

	//子孫が少ない場合
	std::sort(population.begin(), population.end(), [](std::shared_ptr<Solution> lhs, std::shared_ptr<Solution> rhs) {
		return lhs->getFitness() < rhs->getFitness();
	});
	std::copy(offspring.rbegin(), offspring.rend(), population.rbegin());
}

template <class ConcreteGeneticOperableProgram>
void GeneticAlgorithm<ConcreteGeneticOperableProgram>::elitistReinsertion(std::vector<std::shared_ptr<Solution>>& population, std::vector<std::shared_ptr<Solution>>& offspring) {
	int size = population.size();
	population.insert(population.end(), offspring.begin(), offspring.end());
	std::sort(population.begin(), population.end(), [](std::shared_ptr<Solution> lhs, std::shared_ptr<Solution> rhs){
		return lhs->getFitness() < rhs->getFitness();
	});
	population.resize(size);
}

template <class ConcreteGeneticOperableProgram>
std::vector<std::shared_ptr<Solution>> GeneticAlgorithm<ConcreteGeneticOperableProgram>::solveImpl(Evaluator& evaluator, TerminationCriteria& termination) {
	//遺伝子の初期化
	std::vector<std::shared_ptr<Solution>> genes(inits.size());
	std::transform(inits.begin(), inits.end(), genes.begin(), [](std::shared_ptr<Solution> init) {
		return std::make_shared<Solution>(*init);
	});

	//初回の評価
	evaluation(evaluator, genes, *getHistory(), randomEngine);
	getHistory()->addGeneration(genes);

	//世代を進める
	while (!termination.meets(evaluator.getEvaluationCount(), *getHistory())) {
		advanceGeneration(evaluator, genes);
		getHistory()->addGeneration(genes);
	}

	//解を良い順に並べて返す
	std::sort(genes.begin(), genes.end(), [](std::shared_ptr<Solution> lhs, std::shared_ptr<Solution> rhs){
		return lhs->getFitness() < rhs->getFitness();
	});
	return genes;
}

template <class ConcreteGeneticOperableProgram>
void GeneticAlgorithm<ConcreteGeneticOperableProgram>::advanceGeneration(Evaluator& evaluator, std::vector<std::shared_ptr<Solution>>& genes) {
	//解のソート
	std::sort(genes.begin(), genes.end(), [](std::shared_ptr<Solution> lhs, std::shared_ptr<Solution> rhs){
		return lhs->getFitness() < rhs->getFitness();
	});

	//適合度の正規化
	//最小1・大きいほど良い
	std::vector<double> weights;
	const double maxFitness = genes.back()->getFitness();
	for (std::shared_ptr<Solution> gene : genes) {
		weights.push_back(maxFitness - gene->getFitness() + 1.0);
	}

	/*for (std::shared_ptr<ProgramRecord> gene : genes) {
		std::cout << gene->getFitness() << " ";
	}
	std::cout << std::endl;*/

	//スケーリング
	scaling(weights);

	/*for (double weight : weights) {
		std::cout << weight << " ";
	}
	std::cout << std::endl;*/

	//子の作成
	std::vector<std::shared_ptr<Solution>> offspring;
	const int reproductionSize = round((double)genes.size() * reproductionRate);
	const int elitismSize = std::min((int)round((double)genes.size() * elitismRate), reproductionSize);
	while (offspring.size() < (unsigned int)(reproductionSize - elitismSize)) {
		//選択
		int parentIndex1 = selection(weights, randomEngine);
		weights.erase(weights.begin() + parentIndex1);
		int parentIndex2 = selection(weights, randomEngine);
		if (parentIndex2 >= parentIndex1) {
			++parentIndex2;
		}
		std::shared_ptr<Solution> parent1 = genes[parentIndex1];
		std::shared_ptr<Solution> parent2 = genes[parentIndex2];

		//子の作成
		std::shared_ptr<Solution> child1 = parent1->createChild();
		std::shared_ptr<Solution> child2 = parent2->createChild();
		parent1->addChild(child2);
		parent2->addChild(child1);

		std::shared_ptr<ConcreteGeneticOperableProgram> geno1 = std::dynamic_pointer_cast<ConcreteGeneticOperableProgram>(child1->getProgram());
		std::shared_ptr<ConcreteGeneticOperableProgram> geno2 = std::dynamic_pointer_cast<ConcreteGeneticOperableProgram>(child2->getProgram());

		//交叉
		if (crossover != "None") {
			geno1->crossover(crossover, *geno2, randomEngine);
		}

		//変異
		if (mutation != "None") {
			geno1->mutation(mutation, randomEngine);
			geno2->mutation(mutation, randomEngine);
		}

		offspring.push_back(child1);
		offspring.push_back(child2);
	}
	for (int i = 0; i < elitismSize; ++i) {
		std::shared_ptr<Solution> parent = genes[i];
		std::shared_ptr<Solution> child = parent->createChild();
		parent->addChild(child);
		offspring.push_back(child);
	}
	assert(offspring.size() == static_cast<unsigned int>(reproductionSize));

	//評価
	evaluation(evaluator, offspring, *getHistory(), randomEngine);

	//再挿入
	const int prevGeneration = genes[0]->getGeneration();
	reinsertion(genes, offspring);

	//旧世代の更新
	for (std::shared_ptr<Solution>& gene : genes) {
		if (gene->getGeneration() == prevGeneration) {
			gene = gene->createChild(false);
		}
	}
	evaluator.nextSetting();
}

#endif /* GENETICALGORITHM_H_ */
