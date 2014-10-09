/*
 * SantaFeTrail.cpp
 *
 *  Created on: 2013/06/30
 *      Author: kryozahiro
 */

#include <cassert>
#include <fstream>
#include <sstream>
#include "SantaFeTrail.h"
using namespace std;

const OperatorNode SantaFeTrail::IF_FOOD_AHEAD = OperatorNode("IF_FOOD_AHEAD", 2, [](vector<double> input, vector<shared_ptr<ExpressionNode>>& children) {
	SantaFeTrail::getAction().push_back(static_cast<double>(Token::TOKEN_IF_FOOD_AHEAD));
	children[0]->operator()(input);
	children[1]->operator()(input);
	return 0;
});
const OperatorNode SantaFeTrail::PROGN2 = OperatorNode("PROGN2", 2, [](vector<double> input, vector<shared_ptr<ExpressionNode>>& children) {
	SantaFeTrail::getAction().push_back(static_cast<double>(Token::TOKEN_PROGN2));
	children[0]->operator()(input);
	children[1]->operator()(input);
	return 0;
});
const OperatorNode SantaFeTrail::PROGN3 = OperatorNode("PROGN3", 3, [](vector<double> input, vector<shared_ptr<ExpressionNode>>& children) {
	SantaFeTrail::getAction().push_back(static_cast<double>(Token::TOKEN_PROGN3));
	children[0]->operator()(input);
	children[1]->operator()(input);
	children[2]->operator()(input);
	return 0;
});
const OperatorNode SantaFeTrail::MOVE = OperatorNode("MOVE", 0, [](vector<double> input, vector<shared_ptr<ExpressionNode>>& children) {
	SantaFeTrail::getAction().push_back(static_cast<double>(Token::TOKEN_MOVE));
	return 0;
});
const OperatorNode SantaFeTrail::LEFT = OperatorNode("LEFT", 0, [](vector<double> input, vector<shared_ptr<ExpressionNode>>& children) {
	SantaFeTrail::getAction().push_back(static_cast<double>(Token::TOKEN_LEFT));
	return 0;
});
const OperatorNode SantaFeTrail::RIGHT = OperatorNode("RIGHT", 0, [](vector<double> input, vector<shared_ptr<ExpressionNode>>& children) {
	SantaFeTrail::getAction().push_back(static_cast<double>(Token::TOKEN_RIGHT));
	return 0;
});

shared_ptr<SantaFeTrail> SantaFeTrail::getRegular() {
	shared_ptr<SantaFeTrail> sft = make_shared<SantaFeTrail>(32, 33, 400);
	sft->field.clear();
	sft->field = {
		0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,
		0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,
		0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,
		0,0,0,1,1,1,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,1,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
		0,0,0,1,1,0,0,1,1,1,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	};
	return sft;
}

vector<double>& SantaFeTrail::getAction() {
	static vector<double> action;
	return action;
}

SantaFeTrail::SantaFeTrail(int width, int height, int maxStep) :
		Problem(ProgramType(ParameterType(0, 0, 1), 0, ParameterType(0, 0, 1), 1)),
		maxStep(maxStep), width(width), height(height) {
}

double SantaFeTrail::evaluate(Program& program) {
	pos = {0, 0};
	dir = Direction::EAST;
	step = 0;
	simField = field;

	getAction() = vector<double>();
	program(vector<double>());
	vector<double> action = getAction();
	while (step < maxStep) {
		double prevStep = step;
		evaluateAction(action);
		if (step == prevStep) {
			break;
		}
	}
	int food = accumulate(simField.begin(), simField.end(), 0);

	//cout << program.toString();
	//cout << food << endl;
	return food;
}

string SantaFeTrail::toString() const {
	string ret;
	return ret;
}

int SantaFeTrail::evaluateAction(const vector<double>& action, int index, bool doAction) {
	if (step >= maxStep or (unsigned int)index >= action.size()) {
		return index;
	}
	//cout << index << ":(" << step << "," << pos.first << "," << pos.second << "," << action[index] << ") ";

	//正面の取得
	pair<int, int> front = pos;
	switch (dir) {
	case Direction::EAST:
		front.first = (pos.first + 1) % width;
		break;
	case Direction::WEST:
		front.first = (pos.first - 1 + width) % width;
		break;
	case Direction::SOUTH:
		front.second = (pos.second + 1) % height;
		break;
	case Direction::NORTH:
		front.second = (pos.second - 1 + width) % height;
		break;
	default:
		assert(false);
	}
	bool isFoodAhead = simField[front.second * width + front.first];

	//アクションの処理
	int token = action[index];
	++index;
	switch (static_cast<Token>(token)) {
	case Token::TOKEN_IF_FOOD_AHEAD:
		index = evaluateAction(action, index, doAction and isFoodAhead);
		index = evaluateAction(action, index, doAction and not isFoodAhead);
		break;
	case Token::TOKEN_PROGN2:
		index = evaluateAction(action, index, doAction);
		index = evaluateAction(action, index, doAction);
		break;
	case Token::TOKEN_PROGN3:
		index = evaluateAction(action, index, doAction);
		index = evaluateAction(action, index, doAction);
		index = evaluateAction(action, index, doAction);
		break;
	case Token::TOKEN_MOVE:
		if (doAction) {
			pos = front;
			simField[pos.second * width + pos.first] = 0;
			++step;
		}
		break;
	case Token::TOKEN_LEFT:
		if (doAction) {
			dir = (Direction)(((int)dir - 1 + 4) % 4);
			++step;
		}
		break;
	case Token::TOKEN_RIGHT:
		if (doAction) {
			dir = (Direction)(((int)dir + 1) % 4);
			++step;
		}
		break;
	default:
		assert(false);
	}
	return index;
}
