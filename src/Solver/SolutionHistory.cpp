/*
 * SolutionHistory.cpp
 *
 *  Created on: 2014/07/24
 *      Author: kryozahiro
 */

#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include "SolutionHistory.h"
using namespace std;

void SolutionHistory::start() {
	timer.start();
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
}

void SolutionHistory::addPopulation(Population& population) {
	int generation = population.front()->getGeneration();
	generationAttr.set(generation);

	for (shared_ptr<Solution>& solution : population) {
		solution->setTime(timer.elapsed());
		BOOST_LOG(solutionLogger) << solution->toString();
		BOOST_LOG(programLogger) << *solution->getProgram();
	}
	records[generation].insert(records[generation].end(), population.begin(), population.end());

	sort(records[generation].begin(), records[generation].end(), [](shared_ptr<Solution>& lhs, shared_ptr<Solution>& rhs) {
		return lhs->getFitness() < rhs->getFitness();
	});
}

int SolutionHistory::getLastGeneration() const {
	if (records.empty()) {
		return -1;
	}
	return records.rbegin()->second.front()->getGeneration();
}

const SolutionHistory::Population& SolutionHistory::getPopulation(int generation) const {
	assert(getLastGeneration() >= 0 and generation >= -(getLastGeneration() + 1));
	return records.at((generation + getLastGeneration() + 1) % (getLastGeneration() + 1));
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

string SolutionHistory::showSummary() const {
	string ret = "gen\tbest\taverage\tsd\n";

	for (const pair<int, Population>& recordPair : records) {
		int generation = recordPair.first;
		const Population& population = recordPair.second;
		assert(population.size() > 0);

		//最小
		double best = population.front()->getFitness();

		//平均
		double average = accumulate(population.begin(), population.end(), 0.0, [](double sum, shared_ptr<Solution> solution) {
			return sum + solution->getFitness();
		}) / (double)population.size();

		//標準偏差
		double sd = accumulate(population.begin(), population.end(), 0.0, [&](double sum, shared_ptr<Solution> solution) {
			return sum + pow(solution->getFitness() - average, 2.0);
		}) / (double)population.size();
		sd = sqrt(sd);

		ret += boost::lexical_cast<string>(generation) + "\t" +
				boost::lexical_cast<string>(best) + "\t" +
				boost::lexical_cast<string>(average) + "\t" +
				boost::lexical_cast<string>(sd) + "\n";
	}
	return ret;
}
