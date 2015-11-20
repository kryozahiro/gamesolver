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
	HomoAdaptor(const HomoAdaptor& homoAdaptor);
	virtual ~HomoAdaptor() = default;

	/// Loggingの実装
	virtual void setLogger(std::shared_ptr<LoggerType>& logger) override;
	virtual void setLoggerEnabled(bool enabled) override;

	/// Gameの実装
	CPPUTIL_CLONEABLE;
	virtual void advanceSetting(int amount) override;
	virtual std::string toString() const override;

	/// Problemの実装
	virtual double evaluate(Program& program) override;

private:
	std::shared_ptr<Game> game;
	int programSize;
};

#endif /* HOMOADAPTOR_H_ */
