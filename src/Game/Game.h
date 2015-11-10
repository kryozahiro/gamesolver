/*
 * Game.h
 *
 *  Created on: 2013/11/19
 *      Author: kryozahiro
 */

#ifndef GAME_H_
#define GAME_H_

#include <memory>
#include "cpputil/Cloneable.h"
#include "cpputil/Logging.h"
#include "cpputil/reflection/Reflection.h"
#include "../Program/Program.h"
#include "../Program/ProgramType.h"

#define GAMESOLVER_GAME_MODULE(Class)\
	CPPUTIL_AT_START(Class) {\
		auto global = cpputil::Reflection::get_mutable_instance().getGlobal();\
		auto theClass = global->addScope(cpputil::Scope::CLASS, #Class);\
		theClass->addFunction("new", std::function<Game*(const boost::property_tree::ptree&, int)>([](const boost::property_tree::ptree& gameTree, int seed) {\
			return new Class(gameTree, seed);\
		}));\
	}

class Game : public cpputil::Cloneable<Game>, public cpputil::Logging {
public:
	Game(std::pair<int, int> programSize, ProgramType programType);
	virtual ~Game() = default;

	//解関数を評価する
	virtual std::vector<double> evaluate(std::vector<Program*>& programs) = 0;

	//次の問題設定に移る
	virtual void advanceSetting(int amount) {};

	//文字列化
	virtual std::string toString() const = 0;

	//解関数の性質
	std::pair<int, int> getProgramSize() const;
	const ProgramType& getProgramType() const;

private:
	std::pair<int, int> programSize;
	ProgramType programType;
};

#endif /* GAME_H_ */
