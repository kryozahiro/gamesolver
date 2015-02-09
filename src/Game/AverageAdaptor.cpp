/*
 * AverageAdaptor.cpp
 *
 *  Created on: 2014/07/10
 *      Author: kryozahiro
 */

#include <algorithm>
#include "AverageAdaptor.h"
using namespace std;
using namespace cpputil;

AverageAdaptor::AverageAdaptor(std::shared_ptr<Game> game, int sampleSize) :
		Game(game->getProgramSize(), game->getProgramType()), game(game), sampleSize(sampleSize) {
	auto log = game->getLogger();
	Game::setLogger(log);
}

void AverageAdaptor::setLogger(std::shared_ptr<LoggerType>& logger) {
	Game::setLogger(logger);
	game->setLogger(logger);
}

void AverageAdaptor::setLoggerEnabled(bool enabled) {
	Game::setLoggerEnabled(enabled);
	game->setLoggerEnabled(enabled);
}

std::vector<double> AverageAdaptor::evaluate(std::vector<Program*>& programs) {
	vector<double> fitness(programs.size(), 0);
	for (int i = 0; i < sampleSize; ++i) {
		const vector<double>& sample = game->evaluate(programs);
		transform(sample.begin(), sample.end(), fitness.begin(), fitness.begin(), [](const double& s, double& f) {
			return s + f;
		});
	}
	return fitness;
}

void AverageAdaptor::nextSetting() {
	game->nextSetting();
}

std::string AverageAdaptor::toString() const {
	return game->toString();
}
