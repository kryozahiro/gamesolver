/*
 * ExpressionNode.h
 *
 *  Created on: 2013/06/19
 *      Author: kryozahiro
 */

#ifndef EXPRESSIONNODE_H_
#define EXPRESSIONNODE_H_

#include <memory>
#include <random>
#include <vector>
#include <string>
#include "cpputil/Cloneable.h"
#include "../ProgramType.h"

class ExpressionNode : public cpputil::Cloneable<ExpressionNode> {
public:
	//木全体を配列にする
	static std::vector<std::shared_ptr<ExpressionNode>> flatten(std::shared_ptr<ExpressionNode> root);

	ExpressionNode() = default;
	ExpressionNode(const ExpressionNode& node);
	ExpressionNode& operator=(const ExpressionNode& node);
	virtual ~ExpressionNode() = default;

	virtual void randomize(const ProgramType& programType, std::mt19937_64& randomEngine) = 0;

	//個別の計算を行う
	virtual double operator()(const std::vector<double>& input) = 0;
	virtual int getArity() const = 0;
	virtual std::string getName() const = 0;

	//親子関係
	ExpressionNode* getParent();
	void setParent(ExpressionNode* parent);
	void addChild(std::shared_ptr<ExpressionNode> child);
	void replaceChild(ExpressionNode& oldChild, std::shared_ptr<ExpressionNode> newChild);
	int getDepth() const;		//ルートまでのパスの長さ
	int getHeight() const;	//葉までの最長パスの長さ

	std::string toString() const;

	//void foreach(std::function<void(int, std::shared_ptr<ExpressionNode>)> fn);

protected:
	//親ノード
	ExpressionNode* parent = nullptr;

	//子ノード
	std::vector<std::shared_ptr<ExpressionNode>> children;
};

#endif /* EXPRESSIONNODE_H_ */
