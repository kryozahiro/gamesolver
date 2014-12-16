/*
 * ConstantNode.h
 *
 *  Created on: 2013/06/23
 *      Author: kryozahiro
 */

#ifndef CONSTANTNODE_H_
#define CONSTANTNODE_H_

#include "ExpressionNode.h"

class ConstantNode : public ExpressionNode {
public:
	ConstantNode(std::pair<double, double> minmax);
	virtual ~ConstantNode() = default;

	//ExpressionNodeの実装
	CPPUTIL_CLONEABLE;
	void randomize(const ProgramType& programType, std::mt19937_64& randomEngine) override;
	double operator()(const std::vector<double>& input) override;
	int getArity() const override;
	std::string getName() const override;

private:
	std::pair<double, double> minmax;
	double constant;
};

#endif /* CONSTANTNODE_H_ */
