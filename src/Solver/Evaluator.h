/*
 * Evaluator.h
 *
 *  Created on: 2014/07/26
 *      Author: kryozahiro
 */

#ifndef EVALUATOR_H_
#define EVALUATOR_H_

#include <boost/log/attributes/mutable_constant.hpp>
#include "../Game/Game.h"

//評価用ファンクタ
class Evaluator {
public:
	Evaluator(Game& game);
	virtual ~Evaluator() = default;

	//評価の実行
	std::vector<double> operator()(std::vector<Program*>& programs);

	std::pair<int, int> getProgramSize();

	int getEvaluateCount();

private:
	Game& game;
	int evaluateCount = 0;
	boost::log::attributes::mutable_constant<int> evaluationAttr;
};

#endif /* EVALUATOR_H_ */
