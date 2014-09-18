/*
 * ProgramType.cpp
 *
 *  Created on: 2013/11/09
 *      Author: kryozahiro
 */

#include <algorithm>
#include "../Util/GenericIo.h"
#include "ProgramType.h"
using namespace std;

ProgramType::ProgramType(const ParameterType& inputType, int inputSize, const ParameterType& outputType, int outputSize)
	: inputType(inputType), inputSize(inputSize), outputType(outputType), outputSize(outputSize) {
}

void ProgramType::read(istream& is) {
	tuple<ParameterType, int, ParameterType, int> t;
	is >> t;
	inputType = std::get<0>(t);
	inputSize = std::get<1>(t);
	outputType = std::get<2>(t);
	outputSize = std::get<3>(t);
}

void ProgramType::write(ostream& os) const {
	tuple<ParameterType, int, ParameterType, int> t(inputType, inputSize, outputType, outputSize);
	os << t;
}

const ParameterType& ProgramType::getInputType() const {
	return inputType;
}

int ProgramType::getInputSize() const {
	return inputSize;
}

const ParameterType& ProgramType::getOutputType() const {
	return outputType;
}

int ProgramType::getOutputSize() const {
	return outputSize;
}

vector<double> ProgramType::clipInput(const vector<double>& input) const {
	vector<double> result(inputSize, 0);
	for (unsigned int i = 0; i < result.size() and i < input.size(); ++i) {
		result[i] = inputType.clip(input[i]);
	}
	return result;
}

bool ProgramType::acceptsInput(const std::vector<double>& input) const {
	return input == clipInput(input);
}

vector<double> ProgramType::clipOutput(const vector<double>& output) const {
	vector<double> result(outputSize, 0);
	for (unsigned int i = 0; i < result.size() and i < output.size(); ++i) {
		result[i] = outputType.clip(output[i]);
	}
	return result;
}

bool ProgramType::acceptsOutput(const std::vector<double>& output) const {
	return output == clipOutput(output);
}
