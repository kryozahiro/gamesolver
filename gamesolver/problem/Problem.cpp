/*
 * Problem.cpp
 *
 *  Created on: 2013/06/25
 *      Author: kryozahiro
 */

#include "Problem.h"

#include <climits>
#include <algorithm>
using namespace std;

Problem::Problem(ProgramType programType) : Game({1, INT_MAX}, programType) {
}

vector<double> Problem::evaluate(vector<Program*>& programs) {
	vector<double> results(programs.size());
	transform(programs.begin(), programs.end(), results.begin(), [&](Program* program) {
		return evaluate(*program);
	});
	return results;
}
