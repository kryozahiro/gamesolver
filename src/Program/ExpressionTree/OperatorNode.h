/*
 * OperatorNode.h
 *
 *  Created on: 2013/06/23
 *      Author: kryozahiro
 */

#ifndef OPERATORNODE_H_
#define OPERATORNODE_H_

#include <functional>
#include "ExpressionNode.h"

class OperatorNode : public ExpressionNode {
public:
	static const OperatorNode ADD, SUB, MUL, DIV, MOD, POW;
	static OperatorNode fromBinaryOperator(std::string name, std::function<double(double, double)> binaryOperator);

	using Op = std::function<double(const std::vector<double>&, std::vector<std::shared_ptr<ExpressionNode>>&)>;
	OperatorNode(std::string name, int arity, Op op);
	virtual ~OperatorNode() = default;

	//ExpressionNodeの実装
	CPPUTIL_CLONEABLE;
	void randomize(const ProgramType& programType, std::mt19937_64& randomEngine) override;
	double operator()(const std::vector<double>& input) override;
	int getArity() const override;
	std::string getName() const override;

private:
	std::string name;
	int arity;
	Op op;
};

#endif /* OPERATORNODE_H_ */
