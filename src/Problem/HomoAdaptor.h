/*
 * HomoAdaptor.h
 *
 *  Created on: 2014/07/01
 *      Author: kryozahiro
 */

#ifndef HOMOADAPTOR_H_
#define HOMOADAPTOR_H_

#include "Problem.h"

class HomoAdaptor : public Problem {
public:
	HomoAdaptor(std::shared_ptr<Game> game, int programSize);
	virtual ~HomoAdaptor() = default;

	//Gameの実装
	CPPUTIL_CLONEABLE;
	virtual std::string toString() const override;
	virtual void setTime(int time) override;
	virtual void setLogger(std::shared_ptr<boost::log::sources::logger>& logger) override;
	virtual void setLoggerEnabled(bool enabled) override;

	//Problemの実装
	virtual double evaluate(Program& program) override;

private:
	std::shared_ptr<Game> game;
	int programSize;
};

#endif /* HOMOADAPTOR_H_ */
