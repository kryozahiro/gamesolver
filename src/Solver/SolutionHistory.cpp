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

void SolutionHistory::addSolution(shared_ptr<Solution> solution) {
	solution->setTime(timer.elapsed());
	records[solution->getGeneration()].push_back(solution);

	generationAttr.set(solution->getGeneration());
	BOOST_LOG(solutionLogger) << solution->toString();
	BOOST_LOG(programLogger) << *solution->getProgram();
}

const vector<shared_ptr<Solution>>& SolutionHistory::getGeneration(int generation) {
	vector<shared_ptr<Solution>>& target = records.at(generation);
	if (!is_sorted(target.begin(), target.end(), lessThanGeneration)) {
		sort(target.begin(), target.end(), lessThanGeneration);
	}
	return target;
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

	for (const pair<int, vector<std::shared_ptr<Solution>>>& recordPair : records) {
		const Generation& generation = recordPair.second;
		assert(generation.size() > 0);

		//最小
		double best = (*min_element(generation.begin(), generation.end(), lessThanFitness))->getFitness();

		//平均
		double average = accumulate(generation.begin(), generation.end(), 0.0, [](double sum, shared_ptr<Solution> solution) {
			return sum + solution->getFitness();
		}) / (double)generation.size();

		//標準偏差
		double sd = accumulate(generation.begin(), generation.end(), 0.0, [&](double sum, shared_ptr<Solution> solution) {
			return sum + pow(solution->getFitness() - average, 2.0);
		}) / (double)generation.size();
		sd = sqrt(sd);

		ret += boost::lexical_cast<string>(generation.front()->getGeneration()) + "\t" +
				boost::lexical_cast<string>(best) + "\t" +
				boost::lexical_cast<string>(average) + "\t" +
				boost::lexical_cast<string>(sd) + "\n";
	}
	return ret;
}

bool SolutionHistory::lessThanGeneration(const std::shared_ptr<Solution>& lhs, const std::shared_ptr<Solution>& rhs) {
	return lhs->getGeneration() < rhs->getGeneration();
}

bool SolutionHistory::lessThanFitness(const std::shared_ptr<Solution>& lhs, const std::shared_ptr<Solution>& rhs) {
	return lhs->getFitness() < rhs->getFitness();
}
