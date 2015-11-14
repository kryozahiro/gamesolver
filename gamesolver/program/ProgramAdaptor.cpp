/*
 * ProgramAdaptor.cpp
 *
 *  Created on: 2014/01/14
 *      Author: kryozahiro
 */

#include "ProgramAdaptor.h"
using namespace std;

ProgramAdaptor::ProgramAdaptor(const ProgramType& programType, shared_ptr<Program> program) : Program(programType) {
	this->program = program;
}

Program* ProgramAdaptor::clone() {
	ProgramAdaptor* adaptor = new ProgramAdaptor(*this);
	adaptor->program = shared_ptr<Program>(program->clone());
	return adaptor;
}

vector<double> ProgramAdaptor::operator()(const vector<double>& input) {
	return input;
}

string ProgramAdaptor::toString() const {
	return program->toString();
}
