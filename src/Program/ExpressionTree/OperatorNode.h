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
	static const OperatorNode ADD, SUB, MUL, DIV, MOD;
	static const OperatorNode POW;
	static const OperatorNode CONCAT;
	static OperatorNode fromBinaryOperator(std::string name, std::function<double(double, double)> binaryOperator);
	static OperatorNode fromVectoredBinaryOperator(std::string name, std::function<std::vector<double>(const std::vector<double>&, const std::vector<double>&)> vectoredBinaryOperator);

	using Op = std::function<std::vector<double>(const std::vector<double>&, std::vector<std::shared_ptr<ExpressionNode>>&)>;
	OperatorNode(std::string name, int arity, Op op);
	~OperatorNode() noexcept {};

	OperatorNode* clone() const;
	void randomize(const ProgramType& programType, std::mt19937_64& randomEngine);

	std::vector<double> operator()(const std::vector<double>& input);
	int getArity() const;
	std::string getName() const;

private:
	std::string name;
	int arity;
	Op op;
};

#endif /* OPERATORNODE_H_ */
