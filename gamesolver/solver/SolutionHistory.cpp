/*
 * SolutionHistory.cpp
 *
 *  Created on: 2014/07/24
 *      Author: kryozahiro
 */

#include "SolutionHistory.h"

#include <algorithm>
#include <boost/log/sources/record_ostream.hpp>
using namespace std;

SolutionHistory::SolutionHistory(int historySize) : historySize(historySize) {
}

void SolutionHistory::start() {
	timer.start();
	summaryLogger.add_attribute("Summary", generationAttr);
	solutionLogger.add_attribute("Solution", generationAttr);
	programLogger.add_attribute("Program", generationAttr);
}

double SolutionHistory::getElapsedTime() const {
	return static_cast<double>(timer.elapsed()) / 1000.0;
}

void SolutionHistory::addSolution(shared_ptr<Solution>& solution) {
	int generation = solution->getGeneration();
	generationAttr.set(generation);

	solution->setTime(timer.elapsed());
	BOOST_LOG(solutionLogger) << solution->toString();
	BOOST_LOG(programLogger) << *solution->getProgram();
	records[solution->getGeneration()].push_back(solution);

	sort(records[generation].begin(), records[generation].end(), [](shared_ptr<Solution>& lhs, shared_ptr<Solution>& rhs) {
		return lhs->getFitness() < rhs->getFitness();
	});
	while (records.size() > (unsigned int)historySize) {
		records.erase(records.begin());
	}
}

void SolutionHistory::addGeneration(Generation& generation) {
	int generationNum = generation.front()->getGeneration();
	generationAttr.set(generationNum);

	for (shared_ptr<Solution>& solution : generation) {
		solution->setTime(timer.elapsed());
		BOOST_LOG(solutionLogger) << solution->toString();
		BOOST_LOG(programLogger) << *solution->getProgram();
	}
	records[generationNum].insert(records[generationNum].end(), generation.begin(), generation.end());

	sort(records[generationNum].begin(), records[generationNum].end(), [](shared_ptr<Solution>& lhs, shared_ptr<Solution>& rhs) {
		return lhs->getFitness() < rhs->getFitness();
	});
	while (records.size() > (unsigned int)historySize) {
		records.erase(records.begin());
	}
	BOOST_LOG(summaryLogger) << showSummary(records[generationNum]);
}

int SolutionHistory::getLastGenerationNum() const {
	if (records.empty()) {
		return -1;
	}
	return records.rbegin()->second.front()->getGeneration();
}

const SolutionHistory::Generation& SolutionHistory::getGeneration(int generation) const {
	int generationSize = getLastGenerationNum() + 1;
	assert(getLastGenerationNum() >= 0 and generation >= -generationSize);
	return records.at((generation + generationSize) % generationSize);
}

string SolutionHistory::toString() const {
	string ret;
	for (auto itr = records.begin(); itr != records.end(); ++itr) {
		for (const shared_ptr<Solution>& record : itr->second) {
			ret += record->toString() + "\n";
		}
	}
	return ret;
}

string SolutionHistory::showRelation() const {
	string ret;
	for (auto itr = records.begin(); itr != records.end(); ++itr) {
		for (const shared_ptr<Solution>& record : itr->second) {
			ret += record->showRelation();
		}
	}
	return ret;
}

double SolutionHistory::getMeanFitness(const Generation& generation) {
	double sum = accumulate(generation.begin(), generation.end(), 0.0, [](double s, const shared_ptr<Solution>& solution) {
		return s + solution->getFitness();
	});
	return sum / static_cast<double>(generation.size());
}

std::string SolutionHistory::showSummary(const Generation& generation) {
	//世代数
	int generationNum = generation.front()->getGeneration();

	//最小
	double best = generation.front()->getFitness();

	//最大
	double worst = generation.back()->getFitness();

	//平均
	double average = getMeanFitness(generation);

	//不偏分散
	double var = accumulate(generation.begin(), generation.end(), 0.0, [&](double sum, shared_ptr<Solution> solution) {
		return sum + pow(solution->getFitness() - average, 2.0);
	}) / static_cast<double>(generation.size() - 1);

	//標準偏差
	double sd = sqrt(var);

	return to_string(generationNum) + "\t" +
			to_string(best) + "\t" +
			to_string(worst) + "\t" +
			to_string(average) + "\t" +
			to_string(sd);
}
