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
using namespace cpputil;

HomoAdaptor::HomoAdaptor(shared_ptr<Game> game, int programSize) :
		Problem(game->getProgramType()), game(game), programSize(programSize) {
	pair<int, int> size = game->getProgramSize();
	assert(size.first <= programSize and programSize <= size.second);
	auto logger = game->getLogger();
	Game::setLogger(logger);
}

HomoAdaptor::HomoAdaptor(const HomoAdaptor& homoAdaptor) :
		Problem(homoAdaptor), game(homoAdaptor.game->clone()), programSize(homoAdaptor.programSize) {
}

void HomoAdaptor::setLogger(std::shared_ptr<LoggerType>& logger) {
	Game::setLogger(logger);
	game->setLogger(logger);
}

void HomoAdaptor::setLoggerEnabled(bool enabled) {
	Game::setLoggerEnabled(enabled);
	game->setLoggerEnabled(enabled);
}

void HomoAdaptor::nextSetting() {
	game->nextSetting();
}

string HomoAdaptor::toString() const {
	return game->toString();
}

double HomoAdaptor::evaluate(Program& program) {
	vector<Program*> programs(programSize, &program);
	const vector<double>& fitness = game->evaluate(programs);
	double total = accumulate(fitness.begin(), fitness.end(), 0.0);
	return total / (double)programSize;
}
