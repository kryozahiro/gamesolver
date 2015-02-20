/*
 * SolutionHistory.h
 *
 *  Created on: 2014/07/24
 *      Author: kryozahiro
 */

#ifndef SOLUTIONHISTORY_H_
#define SOLUTIONHISTORY_H_

#include <map>
#include <boost/log/sources/logger.hpp>
#include <boost/log/attributes/mutable_constant.hpp>
#include "Solution.h"
#include "CppUtil/Timer.h"

class SolutionHistory {
public:
	typedef std::vector<std::shared_ptr<Solution>> Population;

	//タイマー
	void start();
	double getElapsedTime() const;

	//解を記録する
	void addSolution(std::shared_ptr<Solution>& solution);
	void addPopulation(Population& population);

	//記録の取得
	int getLastGeneration() const;
	const Population& getPopulation(int generation) const;

	//文字列表現
	std::string toString() const;
	std::string showRelation() const;
	std::string showSummary() const;

private:
	//解の記録
	cpputil::Timer timer;
	std::map<int, Population> records;

	//ロギング
	boost::log::sources::logger solutionLogger;
	boost::log::sources::logger programLogger;
	boost::log::attributes::mutable_constant<int> generationAttr = boost::log::attributes::mutable_constant<int>(0);
};

#endif /* SOLUTIONHISTORY_H_ */
