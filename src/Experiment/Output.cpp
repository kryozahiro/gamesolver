/*
 * Output.cpp
 *
 *  Created on: 2015/07/22
 *      Author: kryozahiro
 */

#include <cassert>
#include <boost/filesystem.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/expressions/predicates.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include "cpputil/GenericIo.h"
#include "Output.h"
using namespace std;
using namespace cpputil;
namespace pt = boost::property_tree;
namespace fs = boost::filesystem;
namespace lg = boost::log;

Output::Output(const boost::property_tree::ptree& outputTree) {
	for (const pt::ptree::value_type& kvp : outputTree) {
		if (kvp.first == "<xmlattr>") {
			continue;
		}
		assert(kvp.first == "Logger");

		const pt::ptree& loggerTree = kvp.second;
		Logger logger;
		logger.range = loggerTree.get<std::pair<int, int>>("Range");
		logger.interval = loggerTree.get<int>("Interval");
		logger.filename = loggerTree.get<string>("Filename");
		assert(logger.interval > 0);

		string target = loggerTree.get<string>("Target");
		if (target == "Summary") {
			summaryLogger = logger;
		} else if (target == "Relation") {
			relationLogger = logger;
		} else if (target == "Evaluation") {
			evaluationLogger = logger;
		} else if (target == "Program") {
			programLogger = logger;
		} else if (target == "Solution") {
			solutionLogger = logger;
		} else if (target == "Validator") {
			validatorLogger = logger;
		}
	}
}

void Output::setSink(int stage, const std::string& stageName) {
	auto core = lg::core::get();

	if (stageName == "SolverStage") {
		core->remove_all_sinks();

		fs::path summaryPath(summaryLogger.filename);
		string fullname = summaryPath.parent_path().generic_string() + string("/s") + to_string(stage) + summaryPath.filename().generic_string();
		lg::add_file_log(lg::keywords::file_name = fullname, lg::keywords::filter = lg::expressions::is_in_range<int>("Summary", 0, INT_MAX));
		lg::add_console_log(std::cerr, lg::keywords::filter = lg::expressions::is_in_range<int>("Summary", 0, INT_MAX));

		fs::path relationPath(relationLogger.filename);
		relationFile = relationPath.parent_path().generic_string() + string("/s") + to_string(stage) + relationPath.filename().generic_string();

		setMutableSink(stage, "Evaluation", evaluationLogger);
		setMutableSink(stage, "Program", programLogger);
		setMutableSink(stage, "Solution", solutionLogger);

	} else if (stageName == "ValidatorStage") {
		core->remove_all_sinks();
		setMutableSink(stage, "Validator", validatorLogger);
	}
}

std::string Output::getRelationFile() const {
	return relationFile;
}

std::pair<int, int> Output::getEvaluationLoggerRange() const {
	return evaluationLogger.range;
}

void Output::setMutableSink(int stage, const std::string& target, Logger& logger) {
	fs::path path(logger.filename);
	for (int i = logger.range.first; i < logger.range.second; i += logger.interval) {
		string fullname = path.parent_path().generic_string() + string("/s") + to_string(stage) + path.stem().generic_string() + to_string(i) + path.extension().generic_string();
		lg::add_file_log(lg::keywords::file_name = fullname, lg::keywords::filter = lg::expressions::is_in_range<int>(target, i, i + 1));
	}
}
