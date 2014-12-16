/*
 * Game.cpp
 *
 *  Created on: 2013/11/19
 *      Author: kryozahiro
 */

#include "Game.h"
using namespace std;

Game::Game(std::pair<int, int> programSize, ProgramType programType) :
		programSize(programSize), programType(programType) {
}

int Game::getTime() const {
	return time;
}

void Game::setTime(int time) {
	this->time = time;
}

pair<int, int> Game::getProgramSize() const {
	return programSize;
}

const ProgramType& Game::getProgramType() const {
	return programType;
}

void Game::setLogger(std::shared_ptr<boost::log::sources::logger>& logger) {
	this->logger = logger;
}

void Game::setLoggerEnabled(bool enabled) {
	if (!logger and enabled) {
		assert(false);
		return;
	}
	loggerEnabled = enabled;
}

bool Game::isLoggerEnabled() const {
	return loggerEnabled;
}

std::shared_ptr<boost::log::sources::logger> Game::getLogger() {
	if (loggerEnabled) {
		return logger;
	} else {
		assert(false);
		return nullptr;
	}
}
