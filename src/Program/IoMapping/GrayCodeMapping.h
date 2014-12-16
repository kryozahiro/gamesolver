/*
 * GrayCodeMapping.h
 *
 *  Created on: 2013/06/18
 *      Author: kryozahiro
 */

#ifndef GRAYCODEMAPPING_H_
#define GRAYCODEMAPPING_H_

#include <iostream>
#include <bitset>
#include "IoMapping.h"

class GrayCodeMapping : public IoMapping {
public:
	//一点交叉
	static void onePointCrossover(GrayCodeMapping& parent1, GrayCodeMapping& parent2, std::mt19937_64& randomEngine);

	//通常の変異
	static void normalMutation(GrayCodeMapping& solution, std::mt19937_64& randomEngine);

	//コンストラクタの継承
	GrayCodeMapping(const ProgramType& programType) : IoMapping(programType) {}
	virtual ~GrayCodeMapping() = default;
	CPPUTIL_CLONEABLE;

private:
	static int encode(int raw);
	static int decode(int gray);
};

#endif /* GRAYCODEMAPPING_H_ */
