/*
 * VariableNode.cpp
 *
 *  Created on: 2013/06/23
 *      Author: kryozahiro
 */

#include <boost/lexical_cast.hpp>
#include "VariableNode.h"
using namespace std;

VariableNode* VariableNode::clone() const {
	return new VariableNode(*this);
}

void VariableNode::randomize(const ProgramType& programType, mt19937_64& randomEngine) {
	uniform_int_distribution<int> dist(0, programType.getInputLength() - 1);
	index = dist(randomEngine);
}

vector<double> VariableNode::operator()(const vector<double>& input) {
	return vector<double>(1, input[index]);
}

int VariableNode::getArity() const {
	return 0;
}

string VariableNode::getName() const {
	return "var" + boost::lexical_cast<string>(index);
}
