/*
 * OperatorNode.cpp
 *
 *  Created on: 2013/06/23
 *      Author: kryozahiro
 */

#include <cassert>
#include <algorithm>
#include "OperatorNode.h"
using namespace std;

const OperatorNode OperatorNode::ADD = OperatorNode::fromBinaryOperator("+", plus<double>());
const OperatorNode OperatorNode::SUB = OperatorNode::fromBinaryOperator("-", minus<double>());
const OperatorNode OperatorNode::MUL = OperatorNode::fromBinaryOperator("*", multiplies<double>());
const OperatorNode OperatorNode::DIV = OperatorNode::fromBinaryOperator("/", [](double lhs, double rhs) {
	if (rhs == 0) {
		return 0.0;
	}
	return lhs / rhs;
});
const OperatorNode OperatorNode::MOD = OperatorNode::fromBinaryOperator("%", [](double lhs, double rhs) {
	if (rhs == 0) {
		return 0.0;
	}
	return (double)((int)lhs % (int)rhs);
});
const OperatorNode OperatorNode::POW = OperatorNode::fromBinaryOperator("^", [](double lhs, double rhs) {
	return pow(lhs, rhs);
});
const OperatorNode OperatorNode::CONCAT = OperatorNode::fromVectoredBinaryOperator("concat", [](const vector<double>& lhs, const vector<double>& rhs) {
	vector<double> concat(lhs);
	concat.insert(concat.end(), rhs.begin(), rhs.end());
	return concat;
});

OperatorNode OperatorNode::fromBinaryOperator(std::string name, std::function<double(double, double)> binaryOperator) {
	return OperatorNode::fromVectoredBinaryOperator(name, [=](const vector<double>& lhs, const vector<double>& rhs) {
		assert(lhs.size() == rhs.size());
		vector<double> sum(lhs.size());
		transform(lhs.begin(), lhs.end(), rhs.begin(), sum.begin(), binaryOperator);
		return sum;
	});
}

OperatorNode OperatorNode::fromVectoredBinaryOperator(string name, function<vector<double>(const vector<double>&, const vector<double>&)> vectoredBinaryOperator) {
	return OperatorNode(name, 2, [=](const vector<double>& input, vector<shared_ptr<ExpressionNode>>& children) {
		return vectoredBinaryOperator(children[0]->operator()(input), children[1]->operator()(input));
	});
}

OperatorNode::OperatorNode(string name, int arity, Op op) {
	this->name = name;
	this->arity = arity;
	this->op = op;
}

OperatorNode* OperatorNode::clone() const {
	return new OperatorNode(*this);
}

void OperatorNode::randomize(const ProgramType& programType, mt19937_64& randomEngine) {
}

vector<double> OperatorNode::operator()(const vector<double>& input) {
	assert((unsigned int)getArity() == children.size());
	return op(input, children);
}

int OperatorNode::getArity() const {
	return arity;
}

std::string OperatorNode::getName() const {
	return name;
}
