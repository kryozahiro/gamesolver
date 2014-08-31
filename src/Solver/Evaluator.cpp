/*
 * Evaluator.cpp
 *
 *  Created on: 2014/07/26
 *      Author: kryozahiro
 */

#include "Evaluator.h"
using namespace std;

Evaluator::Evaluator(Game& game) : game(game), evaluationAttr(evaluateCount) {
	game.getLogger()->add_attribute("Evaluation", evaluationAttr);
}

std::vector<double> Evaluator::operator()(std::vector<Program*>& programs) {
	const std::vector<double> fitness = game.evaluate(programs);
	++evaluateCount;
	evaluationAttr.set(evaluateCount);
	return fitness;
}

std::pair<int, int> Evaluator::getProgramSize() {
	return game.getProgramSize();
}

int Evaluator::getEvaluateCount() {
	return evaluateCount;
}

/*void Evaluator::setEvaluateCount(int evaluateCount) {
	this->evaluateCount = evaluateCount;
}*/
