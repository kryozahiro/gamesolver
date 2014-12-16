/*
 * Regression.h
 *
 *  Created on: 2013/06/15
 *      Author: kryozahiro
 */

#ifndef REGRESSION_H_
#define REGRESSION_H_


#include <boost/property_tree/ptree.hpp>
#include "Problem.h"
#include "DataSet.h"

class Regression : public Problem {
public:
	Regression(const DataSet& dataSet);
	Regression(const boost::property_tree::ptree& gameTree, std::mt19937_64& randomEngine);
	virtual ~Regression() = default;

	//Gameの実装
	CPPUTIL_CLONEABLE;
	virtual std::string toString() const override;

	//Problemの実装
	virtual double evaluate(Program& program) override;

private:
	DataSet dataSet;

	static DataSet fromPtree(const boost::property_tree::ptree& gameTree, std::mt19937_64& randomEngine);
};

#endif /* REGRESSION_H_ */
