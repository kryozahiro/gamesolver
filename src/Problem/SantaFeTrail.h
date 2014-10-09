/*
 * SantaFeTrail.h
 *
 *  Created on: 2013/05/13
 *      Author: kryozahiro
 */

#ifndef SANTAFETRAIL_H_
#define SANTAFETRAIL_H_

#include "Problem.h"
#include "../Program/ExpressionTree/OperatorNode.h"

class SantaFeTrail : public Problem {
public:
	static const OperatorNode IF_FOOD_AHEAD, PROGN2, PROGN3, MOVE, LEFT, RIGHT;

	static std::shared_ptr<SantaFeTrail> getRegular();
	static std::vector<double>& getAction();

	//アクションの表現に使用する字句
	enum class Token {
		TOKEN_IF_FOOD_AHEAD,
		TOKEN_PROGN2,
		TOKEN_PROGN3,
		TOKEN_MOVE,
		TOKEN_LEFT,
		TOKEN_RIGHT
	};

	SantaFeTrail(int width, int height, int maxStep);
	virtual ~SantaFeTrail() = default;

	virtual double evaluate(Program& program) override;
	virtual std::string toString() const override;

private:
	//方向
	enum class Direction {
		NORTH = 0,
		EAST = 1,
		SOUTH = 2,
		WEST = 3,
	};

	int maxStep = 0;
	int width = 0;
	int height = 0;
	std::vector<int> field;

	//エージェント
	std::pair<int, int> pos;
	Direction dir = Direction::EAST;
	int step = 0;
	std::vector<int> simField;

	int evaluateAction(const std::vector<double>& action, int index = 0, bool doAction = true);
};

#endif /* SANTAFETRAIL_H_ */
