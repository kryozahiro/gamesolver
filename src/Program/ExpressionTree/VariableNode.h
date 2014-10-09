/*
 * VariableNode.h
 *
 *  Created on: 2013/06/23
 *      Author: kryozahiro
 */

#ifndef VARIABLENODE_H_
#define VARIABLENODE_H_

#include "ExpressionNode.h"

class VariableNode : public ExpressionNode {
public:
	virtual ~VariableNode() = default;

	//ExpressionNodeの実装
	VariableNode* clone() const override;
	void randomize(const ProgramType& programType, std::mt19937_64& randomEngine) override;
	double operator()(const std::vector<double>& input) override;
	int getArity() const override;
	std::string getName() const override;

private:
	int index = 0;
};

#endif /* VARIABLENODE_H_ */
