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
	~ConstantNode() noexcept {};

	ConstantNode* clone() const;
	void randomize(const ProgramType& programType, std::mt19937_64& randomEngine);

	std::vector<double> operator()(const std::vector<double>& input);
	int getArity() const;
	std::string getName() const;

private:
	std::pair<double, double> minmax;
	double constant;
};

#endif /* CONSTANTNODE_H_ */
