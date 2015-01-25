/*
 * Program.cpp
 *
 *  Created on: 2015/01/24
 *      Author: kryozahiro
 */

#include "Program.h"
using namespace std;

Program::Program(const ProgramType& programType) : programType(programType) {
}

void Program::read(std::istream& is) {
	throw "unimplemented";
}

void Program::write(std::ostream& os) const {
	os << "class Program" << std::endl;
}

const ProgramType& Program::getProgramType() const {
	return programType;
}
