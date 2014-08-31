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
	~VariableNode() noexcept {};

	VariableNode* clone() const;
	void randomize(const ProgramType& programType, std::mt19937_64& randomEngine);

	std::vector<double> operator()(const std::vector<double>& input);
	int getArity() const;
	std::string getName() const;

private:
	int index = 0;
};

#endif /* VARIABLENODE_H_ */
