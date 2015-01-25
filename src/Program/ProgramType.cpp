/*
 * ProgramType.cpp
 *
 *  Created on: 2013/11/09
 *      Author: kryozahiro
 */

#include <tuple>
#include "CppUtil/GenericIo.h"
#include "ProgramType.h"
using namespace std;

ProgramType::ProgramType(const DataType& inputType, const DataType& outputType)
	: inputType(inputType), outputType(outputType) {
}

void ProgramType::read(istream& is) {
	tuple<DataType, DataType> t;
	is >> t;
	inputType = get<0>(t);
	outputType = get<1>(t);
}

void ProgramType::write(ostream& os) const {
	tuple<DataType, DataType> t(inputType, outputType);
	os << t;
}

const DataType& ProgramType::getInputType() const {
	return inputType;
}

const DataType& ProgramType::getOutputType() const {
	return outputType;
}
