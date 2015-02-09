/*
 * Evaluator.cpp
 *
 *  Created on: 2014/07/26
 *      Author: kryozahiro
 */

#include <omp.h>
#include "Evaluator.h"
#include "CppUtil/AlgorithmUtil.h"
using namespace std;
using namespace cpputil;

void Evaluator::problemEvaluation(Evaluator& evaluator, std::vector<std::shared_ptr<Solution>>& solutions, SolutionHistory& solutionHistory, std::mt19937_64& randomEngine) {
	assert(evaluator.getProgramSize().first == 1);

	#pragma omp parallel
	{

	#pragma omp critical
	{
		evaluator.parallelGames[omp_get_thread_num()] = shared_ptr<Game>(evaluator.game->clone());
		shared_ptr<boost::log::sources::logger> parallelLog(new boost::log::sources::logger);
		parallelLog->add_attribute(evaluator.loggerName, evaluator.parallelAttrs[omp_get_thread_num()]);
		evaluator.parallelGames[omp_get_thread_num()]->setLogger(parallelLog);
	}

	#pragma omp for schedule(dynamic) ordered
	for (unsigned int i = 0; i < solutions.size(); ++i) {
		/*if (gene->getFitness() != DBL_MAX) {
			continue;
		}*/
		std::vector<Program*> programs(1, &*solutions[i]->getProgram());
		std::vector<double> fitness = evaluator(programs);
		solutions[i]->setFitness(fitness[0]);
	}

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
	} while (cpputil::next_combination(combination.begin(), combination.begin() + evaluationSize, combination.end()));

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

Evaluator::Evaluator(std::shared_ptr<Game> game, std::string loggerName, std::pair<int, int> loggerRange) :
		game(game),
		loggerName(loggerName),
		loggerRange(loggerRange),
		evaluationAttr(evaluationCount),
		parallelGames(omp_get_num_procs()),
		parallelAttrs(omp_get_num_procs(), boost::log::attributes::mutable_constant<int>(evaluationCount)) {
	shared_ptr<boost::log::sources::logger> log(new boost::log::sources::logger);
	log->add_attribute(loggerName, evaluationAttr);
	game->setLogger(log);
}

std::vector<double> Evaluator::operator()(std::vector<Program*>& programs) {
	Game& myGame = omp_in_parallel() ? *parallelGames[omp_get_thread_num()] : *this->game;
	boost::log::attributes::mutable_constant<int>& myAttr = omp_in_parallel() ? parallelAttrs[omp_get_thread_num()] : this->evaluationAttr;

	#pragma omp critical
	{
	if (loggerRange.first <= evaluationCount and evaluationCount < loggerRange.second) {
		myGame.setLoggerEnabled(true);
	} else {
		myGame.setLoggerEnabled(false);
	}
	myAttr.set(evaluationCount);
	++evaluationCount;
	}

	std::vector<double> fitness = myGame.evaluate(programs);
	return fitness;
}

int Evaluator::getEvaluationCount() const {
	return evaluationCount;
}

void Evaluator::nextSetting() {
	game->nextSetting();
}

std::pair<int, int> Evaluator::getProgramSize() {
	return game->getProgramSize();
}
