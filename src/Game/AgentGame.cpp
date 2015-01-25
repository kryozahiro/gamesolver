/*
 * AgentGame.cpp
 *
 *  Created on: 2013/11/20
 *      Author: kryozahiro
 */

#include <cassert>
#include "AgentGame.h"
using namespace std;

AgentGame::AgentGame(std::pair<int, int> programSize, ProgramType programType, int maxStep) :
		Game(programSize, programType), maxStep(maxStep) {
}

vector<double> AgentGame::evaluate(vector<Program*>& programs) {
	assert((unsigned int)getProgramSize().first <= programs.size() && programs.size() <= (unsigned int)getProgramSize().second);

	//初期化
	step = 0;
	initEvaluation(programs.size());

	//実行
	while (step < maxStep) {
		for (unsigned int i = 0; i < programs.size(); ++i) {
			vector<double> input = percept(i);
			assert(getProgramType().getInputType().accepts(input));

			vector<double> output = programs[i]->operator()(input);
			assert(getProgramType().getOutputType().accepts(output));

			affect(i, output);
		}
		update();
		++step;
	}

	//結果の取得
	vector<double> fitness = getFitness();
	assert(fitness.size() == programs.size());
	return fitness;
}

int AgentGame::getStep() const {
	return step;
}

int AgentGame::getMaxStep() const {
	return maxStep;
}

void AgentGame::setMaxStep(int maxStep) {
	if (step < maxStep) {
		this->maxStep = maxStep;
	} else {
		this->maxStep = step;
	}
}
