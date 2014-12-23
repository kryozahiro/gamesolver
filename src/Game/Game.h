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
#include "CppUtil/VirtualConstructable.h"
#include "../Program/Program.h"
#include "../Program/ProgramType.h"

class Game : public cpputil::Cloneable<Game> {
public:
	Game(std::pair<int, int> programSize, ProgramType programType);
	virtual ~Game() = default;

	//解関数を評価する
	virtual std::vector<double> evaluate(std::vector<Program*>& programs) = 0;

	//文字列化
	virtual std::string toString() const = 0;

	//時間の設定
	virtual int getTime() const;
	virtual void setTime(int time);

	//解関数の性質
	std::pair<int, int> getProgramSize() const;
	const ProgramType& getProgramType() const;

	//ロガーの設定
	virtual void setLogger(std::shared_ptr<boost::log::sources::logger>& logger);
	virtual void setLoggerEnabled(bool enabled);

protected:
	//ロガーの取得
	bool isLoggerEnabled() const;
	std::shared_ptr<boost::log::sources::logger> getLogger();

private:
	int time = 0;

	std::pair<int, int> programSize;
	ProgramType programType;

	std::shared_ptr<boost::log::sources::logger> logger;
	bool loggerEnabled = false;
};

#endif /* GAME_H_ */
