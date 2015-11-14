/*
 * AverageAdaptor.cpp
 *
 *  Created on: 2014/07/10
 *      Author: kryozahiro
 */

#include "AverageAdaptor.h"

#include <algorithm>
using namespace std;
using namespace cpputil;

AverageAdaptor::AverageAdaptor(std::shared_ptr<Game> game, int sampleSize) :
		Game(game->getProgramSize(), game->getProgramType()), sampleSize(sampleSize) {
	auto log = game->getLogger();
	Game::setLogger(log);

	games.push_back(game);
	for (int i = 1; i < sampleSize; ++i) {
		shared_ptr<Game> clonedGame(games[i-1]->clone());
		clonedGame->advanceSetting(1);
		games.push_back(clonedGame);
	}
}

AverageAdaptor::AverageAdaptor(const AverageAdaptor& averageAdaptor) :
		Game(averageAdaptor.getProgramSize(), averageAdaptor.getProgramType()), sampleSize(averageAdaptor.sampleSize) {
	for (const shared_ptr<Game>& game : averageAdaptor.games) {
		games.emplace_back(game->clone());
	}
}

void AverageAdaptor::setLogger(std::shared_ptr<LoggerType>& logger) {
	Game::setLogger(logger);
	for (shared_ptr<Game>& game : games) {
		game->setLogger(logger);
	}
}

void AverageAdaptor::setLoggerEnabled(bool enabled) {
	Game::setLoggerEnabled(enabled);
	for (shared_ptr<Game>& game : games) {
		game->setLoggerEnabled(enabled);
	}
}

std::vector<double> AverageAdaptor::evaluate(std::vector<Program*>& programs) {
	vector<double> fitness(programs.size(), 0);
	for (int i = 0; i < sampleSize; ++i) {
		const vector<double>& sample = games[i]->evaluate(programs);
		transform(sample.begin(), sample.end(), fitness.begin(), fitness.begin(), [](const double& s, double& f) {
			return s + f;
		});
	}
	for (double& fit : fitness) {
		fit /= static_cast<double>(sampleSize);
	}
	return fitness;
}

void AverageAdaptor::advanceSetting(int amount) {
	for (shared_ptr<Game>& game : games) {
		game->advanceSetting(amount * games.size());
	}
}

std::string AverageAdaptor::toString() const {
	return games.front()->toString();
}
