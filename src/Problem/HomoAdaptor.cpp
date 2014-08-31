/*
 * HomoAdaptor.cpp
 *
 *  Created on: 2014/07/01
 *      Author: kryozahiro
 */

#include <cassert>
#include <algorithm>
#include "HomoAdaptor.h"
using namespace std;

HomoAdaptor::HomoAdaptor(shared_ptr<Game> game, int programSize) :
		Problem(game->getProgramType()), game(game), programSize(programSize) {
	pair<int, int> size = game->getProgramSize();
	assert(size.first <= programSize and programSize <= size.second);
}

string HomoAdaptor::toString() const {
	return game->toString();
}

double HomoAdaptor::evaluate(Program& program) {
	vector<Program*> programs(programSize, &program);
	const vector<double>& scores = game->evaluate(programs);
	double total = accumulate(scores.begin(), scores.end(), 0.0);
	return total / (double)programSize;
}
