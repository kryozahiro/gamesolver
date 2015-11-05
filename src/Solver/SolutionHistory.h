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
#include "cpputil/Timer.h"

class SolutionHistory {
public:
	typedef std::vector<std::shared_ptr<Solution>> Generation;

	SolutionHistory(int historySize);

	//タイマー
	void start();
	double getElapsedTime() const;

	//解を記録する
	void addSolution(std::shared_ptr<Solution>& solution);
	void addGeneration(Generation& generation);

	//記録の取得
	int getLastGenerationNum() const;
	const Generation& getGeneration(int generation) const;

	//文字列表現
	std::string toString() const;
	std::string showRelation() const;

	static double getMeanFitness(const Generation& generation);
	static std::string showSummary(const Generation& generation);

private:
	//解の記録
	cpputil::Timer timer;
	std::map<int, Generation> records;
	const int historySize;

	//ロギング
	boost::log::sources::logger summaryLogger;
	boost::log::sources::logger solutionLogger;
	boost::log::sources::logger programLogger;
	boost::log::attributes::mutable_constant<int> generationAttr = boost::log::attributes::mutable_constant<int>(0);
};

#endif /* SOLUTIONHISTORY_H_ */
