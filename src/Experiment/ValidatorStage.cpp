/*
 * ValidatorStage.cpp
 *
 *  Created on: 2015/07/22
 *      Author: kryozahiro
 */

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include "CppUtil/GenericIo.h"
#include "../Solver/Evaluator.h"
#include "ValidatorStage.h"
using namespace std;
using namespace cpputil;
namespace ac = boost::accumulators;

ValidatorStage::ValidatorStage(const boost::property_tree::ptree& config, const boost::property_tree::ptree& validatorStageTree, std::shared_ptr<Game>& game) : game(game) {
	times = validatorStageTree.get<int>("Times");
}

void ValidatorStage::operator()(std::vector<std::shared_ptr<Solution>>& solutions, std::mt19937_64& randomEngine) {
	cerr << "Validation:" << endl;
	cerr << "times = " << times << endl;
	cerr << "run\tfitness" << endl;
	validate(solutions);
	cerr << endl;
}

void ValidatorStage::validate(std::vector<std::shared_ptr<Solution>>& solutions) {
	Evaluator evaluator(game, "Validator", pair<int, int>(0, times));

	//各評価位置における評価値の統計情報
	vector<ac::accumulator_set<double, ac::stats<ac::tag::min, ac::tag::mean, ac::tag::variance>>> stats(game->getProgramSize().first);
	vector<int> minIndex(evaluator.getProgramSize().first);

	for (int i = 0; i < times; ++i) {
		//プログラムの選択
		vector<Program*> result(evaluator.getProgramSize().first);
		for (unsigned int k = 0; k < result.size(); ++k) {
			result[k] = &*(solutions[k]->getProgram());
		}

		//評価の実行
		vector<double> fitness = evaluator(result);
		evaluator.nextSetting();
		cerr << i << "\t" << fitness << endl;

		//統計情報の収集
		for (unsigned int k = 0; k < fitness.size(); ++k) {
			stats[k](fitness[k]);
			if (ac::min(stats[k]) == fitness[k]) {
				minIndex[k] = i;
			}
		}
	}

	//統計情報の表示
	cerr << "(bestIndex best mean sd) of each position:" << endl;
	for (unsigned int i = 0; i < stats.size(); ++i) {
		cout << minIndex[i] << "\t" << ac::min(stats[i]) << "\t" << ac::mean(stats[i]) << "\t" << sqrt((double)ac::variance(stats[i])) << "\t";
	}
	cout << endl;
}
