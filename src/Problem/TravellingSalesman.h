/*
 * TravellingSalesman.h
 *
 *  Created on: 2013/05/13
 *      Author: kryozahiro
 */

#ifndef TRAVELLINGSALESMAN_H_
#define TRAVELLINGSALESMAN_H_

#include "Problem.h"

class TravellingSalesman : public Problem {
public:
	TravellingSalesman(int size, int seed);
	virtual ~TravellingSalesman() = default;
	CPPUTIL_CLONEABLE;

	//解は順序表現で表す
	virtual double evaluate(Program& program) override;
	virtual std::string toString() const override;

private:
	//平面巡回セールスマン問題
	std::vector<std::pair<double, double>> cities;
};

#endif /* TRAVELLINGSALESMAN_H_ */
