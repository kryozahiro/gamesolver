/*
 * Output.h
 *
 *  Created on: 2015/07/22
 *      Author: kryozahiro
 */

#ifndef GAMESOLVER_EXPERIMENT_OUTPUT_H_
#define GAMESOLVER_EXPERIMENT_OUTPUT_H_

#include <boost/property_tree/ptree.hpp>

class Output {
public:
	//出力方式の読み込み
	Output(const boost::property_tree::ptree& outputTree);
	void setSink(int stage, const std::string& stageName);

	std::string getRelationFile() const;
	std::pair<int, int> getEvaluationLoggerRange() const;

private:
	struct Logger {
		std::pair<int, int> range;
		int interval;
		std::string filename;
	};

	void setMutableSink(int stage, const std::string& target, Logger& logger);

	std::string relationFile;

	Logger summaryLogger;
	Logger relationLogger;
	Logger evaluationLogger;
	Logger programLogger;
	Logger solutionLogger;
	Logger validatorLogger;
};

#endif /* GAMESOLVER_EXPERIMENT_OUTPUT_H_ */
