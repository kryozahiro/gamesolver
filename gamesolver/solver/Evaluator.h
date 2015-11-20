/*
 * Evaluator.h
 *
 *  Created on: 2014/07/26
 *      Author: kryozahiro
 */

#ifndef EVALUATOR_H_
#define EVALUATOR_H_

#include <boost/log/attributes/mutable_constant.hpp>

#include "../game/Game.h"
#include "SolutionHistory.h"

/// 評価用ファンクタ
class Evaluator {
public:
	using Evaluation = std::function<void(Evaluator& evaluator, std::vector<std::shared_ptr<Solution>>& solutions, SolutionHistory& solutionHistory, std::mt19937_64& randomEngine)>;
	static void problemEvaluation(Evaluator& evaluator, std::vector<std::shared_ptr<Solution>>& solutions, SolutionHistory& solutionHistory, std::mt19937_64& randomEngine);
	//TODO: problemhillclimbevaluation?
	static void allVsAllEvaluation(Evaluator& evaluator, std::vector<std::shared_ptr<Solution>>& solutions, SolutionHistory& solutionHistory, std::mt19937_64& randomEngine);
	static void allVsBestEvaluation(Evaluator& evaluator, std::vector<std::shared_ptr<Solution>>& solutions, SolutionHistory& solutionHistory, std::mt19937_64& randomEngine);
	static void partitioningEvaluation(Evaluator& evaluator, std::vector<std::shared_ptr<Solution>>& solutions, SolutionHistory& solutionHistory, std::mt19937_64& randomEngine);

	Evaluator(std::shared_ptr<Game> game, std::string loggerName, std::pair<int, int> loggerRange);

	/// 評価の実行
	std::vector<double> operator()(std::vector<Program*>& programs);

	/// 評価を実行した回数
	int getEvaluationCount() const;

	/// 次の問題設定に移る
	void nextSetting();

	/// 解関数の性質
	std::pair<int, int> getProgramSize() const;

private:
	std::shared_ptr<Game> game;
	std::string loggerName;
	std::pair<int, int> loggerRange;

	/// 評価回数
	int evaluationCount = 0;
	boost::log::attributes::mutable_constant<int> evaluationAttr;

	/// 並列化
	std::vector<std::shared_ptr<Game>> parallelGames;
	std::vector<boost::log::attributes::mutable_constant<int>> parallelAttrs;
};

#endif /* EVALUATOR_H_ */
