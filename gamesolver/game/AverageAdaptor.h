/*
 * AverageAdaptor.h
 *
 *  Created on: 2014/07/10
 *      Author: kryozahiro
 */

#ifndef AVERAGEADAPTOR_H_
#define AVERAGEADAPTOR_H_

#include "Game.h"

class AverageAdaptor : public Game {
public:
	AverageAdaptor(std::shared_ptr<Game> game, int sampleSize);
	AverageAdaptor(const AverageAdaptor& averageAdaptor);
	virtual ~AverageAdaptor() = default;

	//Loggingの実装
	virtual void setLogger(std::shared_ptr<LoggerType>& logger) override;
	virtual void setLoggerEnabled(bool enabled) override;

	//Gameの実装
	CPPUTIL_CLONEABLE;
	virtual std::vector<double> evaluate(std::vector<Program*>& programs) override;
	virtual void advanceSetting(int amount) override;
	virtual std::string toString() const override;

private:
	std::vector<std::shared_ptr<Game>> games;
	int sampleSize;
};

#endif /* AVERAGEADAPTOR_H_ */
