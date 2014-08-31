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
	virtual ~AverageAdaptor() = default;

	//Gameの実装
	virtual std::vector<double> evaluate(std::vector<Program*>& programs) override;
	virtual std::string toString() const override;

private:
	std::shared_ptr<Game> game;
	int sampleSize;
};

#endif /* AVERAGEADAPTOR_H_ */
