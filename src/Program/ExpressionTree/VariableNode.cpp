/*
 * VariableNode.cpp
 *
 *  Created on: 2013/06/23
 *      Author: kryozahiro
 */

#include <boost/lexical_cast.hpp>
#include "VariableNode.h"
using namespace std;

void VariableNode::randomize(const ProgramType& programType, mt19937_64& randomEngine) {
	uniform_int_distribution<int> dist(0, programType.getInputType().getSize() - 1);
	index = dist(randomEngine);
}

double VariableNode::operator()(const vector<double>& input) {
	return input[index];
}

int VariableNode::getArity() const {
	return 0;
}

string VariableNode::getName() const {
	return "var" + boost::lexical_cast<string>(index);
}
