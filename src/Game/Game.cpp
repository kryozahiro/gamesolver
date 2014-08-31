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

pair<int, int> Game::getProgramSize() const {
	return programSize;
}

const ProgramType& Game::getProgramType() const {
	return programType;
}

void Game::setLogger(std::shared_ptr<boost::log::sources::logger>& logger) {
	this->logger = logger;
}

std::shared_ptr<boost::log::sources::logger> Game::getLogger() {
	return logger;
}
