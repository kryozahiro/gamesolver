/*
 * ConstantNode.cpp
 *
 *  Created on: 2013/06/23
 *      Author: kryozahiro
 */

#include <boost/lexical_cast.hpp>
#include "ConstantNode.h"
using namespace std;

ConstantNode::ConstantNode(pair<double, double> minmax) : minmax(minmax), constant(minmax.first) {
}

void ConstantNode::randomize(const ProgramType& programType, mt19937_64& randomEngine) {
	uniform_real_distribution<double> dist(minmax.first, minmax.second);
	constant = dist(randomEngine);
}

double ConstantNode::operator()(const vector<double>& input) {
	return constant;
}

int ConstantNode::getArity() const {
	return 0;
}

string ConstantNode::getName() const {
	return boost::lexical_cast<string>(constant);
}
