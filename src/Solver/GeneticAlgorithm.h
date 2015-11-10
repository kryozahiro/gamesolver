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
#include "Solver.h"

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

GAMESOLVER_SOLVER_MODULE(GeneticAlgorithm);

#endif /* GENETICALGORITHM_H_ */
