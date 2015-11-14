/*
 * Problem.h
 *
 *  Created on: 2013/05/13
 *      Author: kryozahiro
 */

#ifndef PROBLEM_H_
#define PROBLEM_H_

#include "../game/Game.h"

//エージェントを個別に評価できるゲーム
class Problem : public Game {
public:
	Problem(ProgramType programType);
	virtual ~Problem() = default;

	//Gameの実装
	virtual std::vector<double> evaluate(std::vector<Program*>& programs) override final;

	//解関数を評価する
	virtual double evaluate(Program& program) = 0;
};

#endif /* PROBLEM_H_ */
