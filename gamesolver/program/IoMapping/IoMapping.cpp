/*
 * IoMapping.cpp
 *
 *  Created on: 2013/06/18
 *      Author: kryozahiro
 */

#include "IoMapping.h"

#include <boost/lexical_cast.hpp>
#include "../../problem/Problem.h"
using namespace std;

IoMapping::IoMapping(const ProgramType& programType) : Program(programType) {
	const DataType& inputType = programType.getInputType();
	const DataType& outputType = programType.getOutputType();
	inputVariety = (inputType.getMax() - inputType.getMin() + 1) / inputType.getUnit();
	outputVariety = (outputType.getMax() - outputType.getMin() + 1) / outputType.getUnit();

	if (inputVariety == 0 or inputType.getSize() == 0) {
		//無引数関数
		inputVariety = 1;
	}

	//全ての入力の組み合わせの数
	int domain = pow(inputVariety, inputType.getSize());
	assert(0 < domain);
	assert(domain < 10000);

	mapping = vector<vector<double>>(domain, vector<double>(outputType.getSize(), 0));
}

vector<double> IoMapping::operator()(const vector<double>& input) {
	int pos = 0;
	for (double inputElement : input) {
		pos *= inputVariety;
		pos += inputElement - getProgramType().getInputType().getMin();
	}
	return mapping[pos];
}

string IoMapping::toString() const {
	string ret = "mapping: ";
	for (const vector<double>& output : mapping) {
		for (double outputElement : output) {
			ret += boost::lexical_cast<string>(outputElement) + "-";
		}
		ret += ", ";
	}
	ret += "\n";
	return ret;
}
