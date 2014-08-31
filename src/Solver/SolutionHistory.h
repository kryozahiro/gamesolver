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
#include "../Util/Timer.h"

class SolutionHistory {
public:
	void start();
	void addSolution(std::shared_ptr<Solution> solution);

	const std::vector<std::shared_ptr<Solution>>& getGeneration(int generation);
	std::string toString() const;
	std::string showRelation() const;
	std::string showSummary() const;

private:
	static bool lessThanGeneration(const std::shared_ptr<Solution>& lhs, const std::shared_ptr<Solution>& rhs);
	static bool lessThanFitness(const std::shared_ptr<Solution>& lhs, const std::shared_ptr<Solution>& rhs);
	Timer timer;

	//解の記録
	typedef std::vector<std::shared_ptr<Solution>> Generation;
	std::map<int, Generation> records;

	//ロギング
	boost::log::sources::logger solutionLogger;
	boost::log::sources::logger programLogger;
	boost::log::attributes::mutable_constant<int> generationAttr = boost::log::attributes::mutable_constant<int>(0);
};

#endif /* SOLUTIONHISTORY_H_ */
