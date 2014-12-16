/*
 * ExpressionTree.h
 *
 *  Created on: 2013/06/18
 *      Author: kryozahiro
 */

#ifndef EXPRESSIONTREE_H_
#define EXPRESSIONTREE_H_

#include <random>
#include "../Program.h"
#include "ExpressionNode.h"
#include "../../Solver/GeneticOperable.h"

//関連
#include "OperatorNode.h"
#include "ConstantNode.h"
#include "VariableNode.h"

class ExpressionTree : public Program, public GeneticOperable<ExpressionTree> {
public:
	static std::vector<std::shared_ptr<ExpressionTree>> generate(const ProgramType& programType, const boost::property_tree::ptree& node, int size, std::mt19937_64& randomEngine);

	//ramped half and halfによる式木の集合の生成
	static std::vector<std::shared_ptr<ExpressionTree>> rampedHalfAndHalf(const ExpressionTree& prototype, int size, int maxDepth, std::mt19937_64& randomEngine);

	//GeneticOperableの実装
	static void normalCrossover(ExpressionTree& parent1, ExpressionTree& parent2, int rootIndex, std::mt19937_64& randomEngine);
	static void normalMutation(ExpressionTree& solution, int rootIndex, std::mt19937_64& randomEngine);

	//コンストラクタ
	//maxDepth: 葉までの最長パスの長さ
	ExpressionTree(const ProgramType& programType, int maxDepth);
	ExpressionTree(const ExpressionTree& tree);
	virtual ~ExpressionTree() = default;

	//Programの実装
	CPPUTIL_CLONEABLE;
	virtual std::vector<double> operator()(const std::vector<double>& input) override;
	virtual std::string toString() const override;

	//GeneticOperableの実装
	virtual void crossover(const std::string& method, ExpressionTree& other, std::mt19937_64& randomEngine) override;
	virtual void mutation(const std::string& method, std::mt19937_64& randomEngine) override;

	//使用するオペレータの設定
	void addOperator(std::shared_ptr<ExpressionNode> op);

	//式木の生成
	void full(std::mt19937_64& randomEngine);
	void grow(std::mt19937_64& randomEngine);

private:
	using MethodMakeNode = std::shared_ptr<ExpressionNode>(ExpressionTree::*)(int, int, std::mt19937_64&);
	std::shared_ptr<ExpressionNode> makeSubTree(int depth, std::mt19937_64& randomEngine, MethodMakeNode method);
	std::shared_ptr<ExpressionNode> makeNodeByGrow(int thisDepth, int maxDepth, std::mt19937_64& randomEngine);
	std::shared_ptr<ExpressionNode> makeNodeByFull(int thisDepth, int maxDepth, std::mt19937_64& randomEngine);

	//問題の仕様
	ProgramType programType;
	int maxDepth = 10;

	//式木
	std::vector<std::shared_ptr<ExpressionNode>> roots;

	//使用するオペレータのリスト
	std::vector<std::shared_ptr<ExpressionNode>> nonterminals;
	std::vector<std::shared_ptr<ExpressionNode>> terminals;
};

#endif /* EXPRESSIONTREE_H_ */
