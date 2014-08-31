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
#include "../Util/Scale.h"
#include "../Util/AlgorithmUtil.h"
#include "Solver.h"
#include "GeneticOperable.h"

#include <iostream>

template <class ConcreteGeneticOperableProgram>
class GeneticAlgorithm : public Solver<Game> {
public:
	using Scaling = std::function<void(std::vector<double>& weights)>;
	using Selection = std::function<int(std::vector<double>& weights, std::mt19937_64& randomEngine)>;
	using Evaluation = std::function<void(Evaluator& evaluator, std::vector<std::shared_ptr<Solution>>& solutions, SolutionHistory& solutionHistory)>;
	using Reinsertion = std::function<void(std::vector<std::shared_ptr<Solution>>& population, std::vector<std::shared_ptr<Solution>>& offspring)>;

	GeneticAlgorithm(const std::vector<std::shared_ptr<Program>>& inits, int seed)
	: inits(inits.size()), randomEngine(seed) {
		for (unsigned int i = 0; i < inits.size(); ++i) {
			this->inits[i] = std::make_shared<Solution>(inits[i]);
			this->inits[i]->setGeneration(0);
		}
	}

	//ptreeからの初期化
	GeneticAlgorithm(boost::property_tree::ptree& node, const std::vector<std::shared_ptr<Program>>& inits)
	: inits(inits.size()) {
		//乱数の初期化
		std::string seed = node.get<std::string>("Seed");
		if (seed == "Random") {
			randomEngine = std::mt19937_64(time(NULL));
		} else {
			randomEngine = std::mt19937_64(atoi(seed.c_str()));
		}

		//初期解の設定
		for (unsigned int i = 0; i < inits.size(); ++i) {
			this->inits[i] = std::make_shared<Solution>(inits[i]);
			this->inits[i]->setGeneration(0);
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
			setEvaluation(problemEvaluation);
		} else if (evaluatorName == "AllVsAllEvaluation") {
			setEvaluation(allVsAllEvaluation);
		} else if (evaluatorName == "AllVsBestEvaluation") {
			setEvaluation(allVsBestEvaluation);
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

	virtual ~GeneticAlgorithm() = default;

	//操作の設定
	void setScaling(Scaling scaling) {
		this->scaling = scaling;
	}
	void setReproductionRate(double reproductionRate) {
		this->reproductionRate = reproductionRate;
	}
	void setElitismRate(double elitismRate) {
		assert(0.0 <= elitismRate and elitismRate <= 1.0);
		this->elitismRate = elitismRate;
	}
	void setSelection(Selection selection) {
		this->selection = selection;
	}
	void setCrossover(std::string crossoverName) {
		this->crossover = crossoverName;
	}
	void setMutation(std::string mutationName) {
		this->mutation = mutationName;
	}
	void setEvaluation(Evaluation evaluation) {
		this->evaluation = evaluation;
	}
	void setReinsertion(Reinsertion reinsertion) {
		this->reinsertion = reinsertion;
	}

	//スケーリング
	static void linearScaling(std::vector<double>& weights) {
		double scale = 2;

		const double max = weights.front();
		double mean = std::accumulate(weights.begin(), weights.end(), 0.0) / weights.size();

		//変換
		if (max == mean) {
			return;
		}
		std::function<double(double)> scaler = Scale::linearScaler(mean, max, scale);
	}
	static void sigmaScaling(std::vector<double>& weights) {
		double scale = 2;

		double mean = std::accumulate(weights.begin(), weights.end(), 0.0) / weights.size();
		double sd = std::accumulate(weights.begin(), weights.end(), 0.0, [&](double sum, double lhs) {
			return sum + (lhs - mean) * (lhs - mean);
		}) / weights.size();
		sd = sqrt(sd);

		//変換
		std::function<double(double)> scaler = Scale::simgaScaler(mean, sd, scale);
	}

	//ルーレット選択
	static int rouletteSelection(std::vector<double> weights, std::mt19937_64& randomEngine) {
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

	//解集合の評価
	static void problemEvaluation(Evaluator& evaluator, std::vector<std::shared_ptr<Solution>>& solutions, SolutionHistory& solutionHistory) {
		assert(evaluator.getProgramSize().first == 1);
		for (std::shared_ptr<Solution>& gene : solutions) {
			/*if (gene->getFitness() != DBL_MAX) {
				continue;
			}*/
			std::vector<Program*> programs(1, &*gene->getProgram());
			std::vector<double> fitness = evaluator(programs);
			gene->setFitness(fitness[0]);
		}
	}
	//TODO: problemhillclimbevaluation?
	static void allVsAllEvaluation(Evaluator& evaluator, std::vector<std::shared_ptr<Solution>>& solutions, SolutionHistory& solutionHistory) {
		const int evaluationSize = evaluator.getProgramSize().first;
		std::vector<Program*> programs(evaluationSize);
		std::vector<double> fitness(solutions.size(), 0);

		//全ての組み合わせを合計
		std::vector<int> combination(solutions.size());
		std::iota(combination.begin(), combination.end(), 0);
		do {
			for (int i = 0; i < evaluationSize; ++i) {
				programs[i] = &*solutions[combination[i]]->getProgram();
			}
			std::vector<double> comb_fitness = evaluator(programs);
			for (int i = 0; i < evaluationSize; ++i) {
				fitness[combination[i]] += comb_fitness[i];
			}
		} while (AlgorithmUtil::next_combination(combination.begin(), combination.begin() + evaluationSize, combination.end()));

		//合計を適応度に設定
		for (unsigned int i = 0; i < fitness.size(); ++i) {
			solutions[i]->setFitness(fitness[i]);
		}
	}
	static void allVsBestEvaluation(Evaluator& evaluator, std::vector<std::shared_ptr<Solution>>& solutions, SolutionHistory& solutionHistory) {
		if (solutions[0]->getGeneration() == 0) {
			allVsAllEvaluation(evaluator, solutions, solutionHistory);
			return;
		}

		const std::vector<std::shared_ptr<Solution>>& latest = solutionHistory.getGeneration(solutions[0]->getGeneration() - 1);

		const int evaluationSize = evaluator.getProgramSize().first;
		std::vector<Program*> programs(evaluationSize);
		std::vector<double> fitness(solutions.size(), 0);

		//全ての組み合わせを合計
		std::vector<int> combination(solutions.size());
		std::iota(combination.begin(), combination.end(), 0);
		do {
			for (int i = 0; i < evaluationSize - 1; ++i) {
				programs[i] = &*solutions[combination[i]]->getProgram();
			}
			programs[evaluationSize - 1] = &*(latest[0]->getProgram());

			std::vector<double> comb_fitness = evaluator(programs);
			for (int i = 0; i < evaluationSize - 1; ++i) {
				fitness[combination[i]] += comb_fitness[i];
			}
		} while (AlgorithmUtil::next_combination(combination.begin(), combination.begin() + evaluationSize - 1, combination.end()));

		//合計を適応度に設定
		for (unsigned int i = 0; i < fitness.size(); ++i) {
			solutions[i]->setFitness(fitness[i]);
		}
	}

	//再挿入
	static void pureReinsertion(std::vector<std::shared_ptr<Solution>>& population, std::vector<std::shared_ptr<Solution>>& offspring) {
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
	//TODO: uniform reinsertion
	static void elitistReinsertion(std::vector<std::shared_ptr<Solution>>& population, std::vector<std::shared_ptr<Solution>>& offspring) {
		int size = population.size();
		population.insert(population.end(), offspring.begin(), offspring.end());
		std::sort(population.begin(), population.end(), [](std::shared_ptr<Solution> lhs, std::shared_ptr<Solution> rhs){
			return lhs->getFitness() < rhs->getFitness();
		});
		population.resize(size);
	}
	//TODO: fitness-based reinsertion

protected:
	virtual std::vector<std::shared_ptr<Solution>> solveImpl(Evaluator& evaluator, int evaluateNum) override {
		//遺伝子の初期化
		std::vector<std::shared_ptr<Solution>> genes(inits.size());
		std::transform(inits.begin(), inits.end(), genes.begin(), [](std::shared_ptr<Solution> init) {
			return std::make_shared<Solution>(*init);
		});

		//初回の評価
		evaluation(evaluator, genes, *getHistory());
		logGenes(genes);

		//世代を進める
		while (evaluator.getEvaluateCount() < evaluateNum) {
			advanceGeneration(evaluator, genes);
			logGenes(genes);
		}

		//解を良い順に並べて返す
		std::sort(genes.begin(), genes.end(), [](std::shared_ptr<Solution> lhs, std::shared_ptr<Solution> rhs){
			return lhs->getFitness() < rhs->getFitness();
		});
		return genes;
	}

private:
	//解の集合をログに追加する
	void logGenes(std::vector<std::shared_ptr<Solution>>& genes) {
		for (std::shared_ptr<Solution>& gene : genes) {
			getHistory()->addSolution(gene);
		}
	}

	//世代を進める
	//評価した回数を返す
	void advanceGeneration(Evaluator& evaluator, std::vector<std::shared_ptr<Solution>>& genes) {
		//解のソート
		std::sort(genes.begin(), genes.end(), [](std::shared_ptr<Solution> lhs, std::shared_ptr<Solution> rhs){
			return lhs->getFitness() < rhs->getFitness();
		});

		//適合度の正規化
		//最小1・大きいほど良い
		std::vector<double> weights;
		double maxFitness = genes.back()->getFitness();
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
		int reproductionSize = round((double)genes.size() * reproductionRate);
		int elitismSize = std::min((int)round((double)genes.size() * elitismRate), reproductionSize);
		for (int i = 0; i < elitismSize; ++i) {
			std::shared_ptr<Solution> parent = genes[i];
			std::shared_ptr<Solution> child = parent->createChild();
			parent->addChild(*child);
			offspring.push_back(child);
		}
		while (offspring.size() < (unsigned int)reproductionSize) {
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
			parent1->addChild(*child2);
			parent2->addChild(*child1);

			std::shared_ptr<ConcreteGeneticOperableProgram> geno1 = std::dynamic_pointer_cast<ConcreteGeneticOperableProgram>(child1->getProgram());
			std::shared_ptr<ConcreteGeneticOperableProgram> geno2 = std::dynamic_pointer_cast<ConcreteGeneticOperableProgram>(child2->getProgram());

			//交叉
			geno1->crossover(crossover, *geno2, randomEngine);

			//変異
			geno1->mutation(mutation, randomEngine);
			geno2->mutation(mutation, randomEngine);

			offspring.push_back(child1);
			offspring.push_back(child2);
		}

		//評価
		evaluation(evaluator, offspring, *getHistory());

		//再挿入
		int prevGeneration = genes[0]->getGeneration();
		reinsertion(genes, offspring);

		//旧世代の更新
		for (std::shared_ptr<Solution>& gene : genes) {
			if (gene->getGeneration() == prevGeneration) {
				gene = gene->createChild(false);
			}
		}
	}

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
	Evaluation evaluation;
	Reinsertion reinsertion = pureReinsertion;
};

#endif /* GENETICALGORITHM_H_ */
