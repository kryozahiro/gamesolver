/*
 * ConstantNode.cpp
 *
 *  Created on: 2013/06/23
 *      Author: kryozahiro
 */

#include <boost/lexical_cast.hpp>
#include "ConstantNode.h"
using namespace std;

ConstantNode::ConstantNode(pair<double, double> minmax) {
	this->minmax = minmax;
	this->constant = minmax.first;
}

ConstantNode* ConstantNode::clone() const {
	return new ConstantNode(*this);
}

void ConstantNode::randomize(const ProgramType& programType, mt19937_64& randomEngine) {
	uniform_real_distribution<double> dist(minmax.first, minmax.second);
	constant = dist(randomEngine);
}

vector<double> ConstantNode::operator()(const vector<double>& input) {
	return vector<double>(1, constant);
}

int ConstantNode::getArity() const {
	return 0;
}

string ConstantNode::getName() const {
	return boost::lexical_cast<string>(constant);
}
