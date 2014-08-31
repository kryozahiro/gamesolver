/*
 * Game.h
 *
 *  Created on: 2013/11/19
 *      Author: kryozahiro
 */

#ifndef GAME_H_
#define GAME_H_

#include <memory>
#include <boost/log/sources/logger.hpp>
#include "../Program/Program.h"
#include "../Program/ProgramType.h"

class Game {
public:
	Game(std::pair<int, int> programSize, ProgramType programType);
	virtual ~Game() = default;

	//解関数を評価する
	virtual std::vector<double> evaluate(std::vector<Program*>& programs) = 0;

	//文字列化
	virtual std::string toString() const = 0;

	//ログの設定
	virtual void setLogger(std::shared_ptr<boost::log::sources::logger>& logger);
	std::shared_ptr<boost::log::sources::logger> getLogger();

	//解関数の性質
	std::pair<int, int> getProgramSize() const;
	const ProgramType& getProgramType() const;

private:
	std::pair<int, int> programSize;
	ProgramType programType;

	std::shared_ptr<boost::log::sources::logger> logger;
};

#endif /* GAME_H_ */
