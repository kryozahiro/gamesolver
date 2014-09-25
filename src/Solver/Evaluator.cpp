/*
 * Evaluator.cpp
 *
 *  Created on: 2014/07/26
 *      Author: kryozahiro
 */

#include "Evaluator.h"
#include "../Util/AlgorithmUtil.h"
using namespace std;

void Evaluator::problemEvaluation(Evaluator& evaluator, std::vector<std::shared_ptr<Solution>>& solutions, SolutionHistory& solutionHistory, std::mt19937_64& randomEngine) {
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

void Evaluator::allVsAllEvaluation(Evaluator& evaluator, std::vector<std::shared_ptr<Solution>>& solutions, SolutionHistory& solutionHistory, std::mt19937_64& randomEngine) {
	const int evaluationSize = evaluator.getProgramSize().first;
	std::vector<double> fitness(solutions.size(), 0);

	//全ての組み合わせを合計
	std::vector<int> combination(solutions.size());
	std::iota(combination.begin(), combination.end(), 0);
	do {
		std::vector<Program*> programs(evaluationSize);
		for (int i = 0; i < evaluationSize; ++i) {
			programs[i] = &*solutions[combination[i]]->getProgram();
		}
		std::vector<double> combFitness = evaluator(programs);
		for (int i = 0; i < evaluationSize; ++i) {
			fitness[combination[i]] += combFitness[i];
		}
	} while (AlgorithmUtil::next_combination(combination.begin(), combination.begin() + evaluationSize, combination.end()));

	//合計を適応度に設定
	for (unsigned int i = 0; i < fitness.size(); ++i) {
		solutions[i]->setFitness(fitness[i]);
	}
}

void Evaluator::allVsBestEvaluation(Evaluator& evaluator, std::vector<std::shared_ptr<Solution>>& solutions, SolutionHistory& solutionHistory, std::mt19937_64& randomEngine) {
	if (solutions[0]->getGeneration() == 0) {
		allVsAllEvaluation(evaluator, solutions, solutionHistory, randomEngine);
		return;
	}
	const int evaluationSize = evaluator.getProgramSize().first;
	const std::vector<std::shared_ptr<Solution>>& latest = solutionHistory.getGeneration(solutions[0]->getGeneration() - 1);

	//前世代の最良解を含むグループを作って評価
	double sum = 0;
	for (unsigned int group = 0; group < solutions.size() / (evaluationSize - 1); ++group) {
		//グループを作成
		std::vector<Program*> programs(evaluationSize);
		for (int i = 0; i < evaluationSize - 1; ++i) {
			programs[i] = &*solutions[group * evaluationSize + i]->getProgram();
		}
		programs[evaluationSize - 1] = &*latest[0]->getProgram();

		//評価
		std::vector<double> fitness = evaluator(programs);
		for (int i = 0; i < evaluationSize - 1; ++i) {
			solutions[group * evaluationSize + i]->setFitness(fitness[i]);
			sum += fitness[i];
		}
	}

	//余りの評価値には平均を設定
	int last = solutions.size() % (evaluationSize - 1);
	double mean = sum / (double)(solutions.size() - last);
	for (int i = 0; i < last; ++i) {
		(*(solutions.end() - 1 - i))->setFitness(mean);
	}
}

void Evaluator::partitioningEvaluation(Evaluator& evaluator, std::vector<std::shared_ptr<Solution>>& solutions, SolutionHistory& solutionHistory, std::mt19937_64& randomEngine) {
	const int evaluationSize = evaluator.getProgramSize().first;
	std::shuffle(solutions.begin(), solutions.end(), randomEngine);

	//グループを作って評価
	double sum = 0;
	for (unsigned int group = 0; group < solutions.size() / evaluationSize; ++group) {
		//グループを作成
		std::vector<Program*> programs(evaluationSize);
		for (int i = 0; i < evaluationSize; ++i) {
			programs[i] = &*solutions[group * evaluationSize + i]->getProgram();
		}

		//評価
		std::vector<double> fitness = evaluator(programs);
		for (int i = 0; i < evaluationSize; ++i) {
			solutions[group * evaluationSize + i]->setFitness(fitness[i]);
			sum += fitness[i];
		}
	}

	//余りの評価値には平均を設定
	int last = solutions.size() % evaluationSize;
	double mean = sum / (double)(solutions.size() - last);
	for (int i = 0; i < last; ++i) {
		(*(solutions.end() - 1 - i))->setFitness(mean);
	}
}

Evaluator::Evaluator(Game& game, std::string loggerName, std::pair<int, int> loggerRange) :
		game(game),
		logger(make_shared<boost::log::sources::logger>()),
		evaluationAttr(evaluateCount),
		loggerRange(loggerRange) {
	logger->add_attribute(loggerName, evaluationAttr);
	game.setLogger(logger);
}

std::vector<double> Evaluator::operator()(std::vector<Program*>& programs) {
	if (loggerRange.first <= evaluateCount and evaluateCount < loggerRange.second) {
		game.setLoggerEnabled(true);
	} else {
		game.setLoggerEnabled(false);
	}
	const std::vector<double> fitness = game.evaluate(programs);
	++evaluateCount;
	evaluationAttr.set(evaluateCount);
	return fitness;
}

int Evaluator::getEvaluateCount() {
	return evaluateCount;
}

std::pair<int, int> Evaluator::getProgramSize() {
	return game.getProgramSize();
}
