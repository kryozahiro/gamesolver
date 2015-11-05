/*
 * ExpressionTree.cpp
 *
 *  Created on: 2013/06/19
 *      Author: kryozahiro
 */

#include <cassert>
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include "cpputil/GenericIo.h"
#include "ExpressionTree.h"
#include "../../Problem/SantaFeTrail.h"
using namespace std;
namespace pt = boost::property_tree;

vector<shared_ptr<ExpressionTree>> ExpressionTree::generate(const ProgramType& programType, const pt::ptree& node, int size, mt19937_64& randomEngine) {
	int maxDepth = node.get<int>("MaxDepth");
	ExpressionTree prototype(programType, maxDepth);

	const pt::ptree& operators = node.get_child("Operators");
	for (const pt::ptree::value_type& kvp : operators) {
		if (kvp.first == "Constant") {
			string rangeStr = kvp.second.data();
			pair<double, double> range = boost::lexical_cast<pair<double, double>>(rangeStr);
			prototype.addOperator(make_shared<ConstantNode>(range));
		} else if (kvp.first == "Variable") {
			prototype.addOperator(make_shared<VariableNode>());
		} else if (kvp.first == "Add") {
			prototype.addOperator(make_shared<OperatorNode>(OperatorNode::ADD));
		} else if (kvp.first == "Sub") {
			prototype.addOperator(make_shared<OperatorNode>(OperatorNode::SUB));
		} else if (kvp.first == "Mul") {
			prototype.addOperator(make_shared<OperatorNode>(OperatorNode::MUL));
		} else if (kvp.first == "Div") {
			prototype.addOperator(make_shared<OperatorNode>(OperatorNode::DIV));
		} else if (kvp.first == "Pow") {
			prototype.addOperator(make_shared<OperatorNode>(OperatorNode::POW));
		} else if (kvp.first == "SantaFeTrailOperators") {
			prototype.addOperator(make_shared<OperatorNode>(SantaFeTrail::IF_FOOD_AHEAD));
			prototype.addOperator(make_shared<OperatorNode>(SantaFeTrail::PROGN2));
			prototype.addOperator(make_shared<OperatorNode>(SantaFeTrail::PROGN3));
			prototype.addOperator(make_shared<OperatorNode>(SantaFeTrail::MOVE));
			prototype.addOperator(make_shared<OperatorNode>(SantaFeTrail::LEFT));
			prototype.addOperator(make_shared<OperatorNode>(SantaFeTrail::RIGHT));
		} else {
			assert(false);
		}
	}

	string method = node.get<string>("Method");
	if (method == "RampedHalfAndHalf") {
		return ExpressionTree::rampedHalfAndHalf(prototype, size, maxDepth, randomEngine);
	} else {
		assert(false);
	}
	return vector<shared_ptr<ExpressionTree>>();
}

vector<shared_ptr<ExpressionTree>> ExpressionTree::rampedHalfAndHalf(const ExpressionTree& prototype, int size, int maxDepth, mt19937_64& randomEngine) {
	vector<shared_ptr<ExpressionTree>> trees;

	int eachSize = size / (maxDepth - 1);
	for (int depth = 2; depth <= maxDepth; ++depth) {
		for (int k = 0; k < eachSize / 2; ++k) {
			shared_ptr<ExpressionTree> full = make_shared<ExpressionTree>(prototype);
			full->maxDepth = depth;
			full->full(randomEngine);
			trees.push_back(full);
		}
		for (int k = 0; k < eachSize - eachSize / 2; ++k) {
			shared_ptr<ExpressionTree> grow = make_shared<ExpressionTree>(prototype);
			grow->maxDepth = depth;
			grow->grow(randomEngine);
			trees.push_back(grow);
		}
	}

	return trees;
}

void ExpressionTree::normalCrossover(ExpressionTree& parent1, ExpressionTree& parent2, int rootIndex, mt19937_64& randomEngine) {
	assert(&parent1 != &parent2);

	//入れ替える部分木を選ぶ
	assert(parent1.roots[rootIndex]);
	std::vector<std::shared_ptr<ExpressionNode>> flat1 = ExpressionNode::flatten(parent1.roots[rootIndex]);
	std::uniform_int_distribution<int> dist1(0, flat1.size() - 1);
	std::shared_ptr<ExpressionNode> target1 = flat1[dist1(randomEngine)];
	int depth1 = target1->getDepth();
	int height1 = target1->getHeight();

	assert(parent2.roots[rootIndex]);
	std::vector<std::shared_ptr<ExpressionNode>> flat2 = ExpressionNode::flatten(parent2.roots[rootIndex]);
	flat2.erase(remove_if(flat2.begin(), flat2.end(), [&](std::shared_ptr<ExpressionNode>& node2) {
		return (depth1 + node2->getHeight() > parent1.maxDepth) or (node2->getDepth() + height1 > parent2.maxDepth);
	}), flat2.end());
	if (flat2.size() == 0) {
		//TODO
		return;
	}
	std::uniform_int_distribution<int> dist2(0, flat2.size() - 1);
	std::shared_ptr<ExpressionNode> target2 = flat2[dist2(randomEngine)];

	//入れ替える
	ExpressionNode* up1 = target1->getParent();
	ExpressionNode* up2 = target2->getParent();
	if (up1 == nullptr) {
		parent1.roots[rootIndex] = target2;
		parent1.roots[rootIndex]->setParent(nullptr);
	} else {
		up1->replaceChild(*target1, target2);
	}
	if (up2 == nullptr) {
		parent2.roots[rootIndex] = target1;
		parent2.roots[rootIndex]->setParent(nullptr);
	} else {
		up2->replaceChild(*target2, target1);
	}
}

void ExpressionTree::normalMutation(ExpressionTree& solution, int rootIndex, std::mt19937_64& randomEngine) {
	std::uniform_int_distribution<int> mutationDist(1, 100);
	if (mutationDist(randomEngine) > 5) {
		return;
	}

	//変異の開始点を選ぶ
	std::vector<std::shared_ptr<ExpressionNode>> flat = ExpressionNode::flatten(solution.roots[rootIndex]);
	std::uniform_int_distribution<int> dist(0, flat.size() - 1);
	std::shared_ptr<ExpressionNode> target = flat[dist(randomEngine)];
	ExpressionNode* up = target->getParent();

	//新しい木を作る
	int maxDepth = solution.maxDepth - target->getDepth();
	std::uniform_int_distribution<int> depthDist(0, maxDepth);
	std::uniform_int_distribution<int> useGrow(0, 1);
	std::shared_ptr<ExpressionNode> subTree;
	if (useGrow(randomEngine)) {
		subTree = solution.makeSubTree(depthDist(randomEngine), randomEngine, &ExpressionTree::makeNodeByFull);
	} else {
		subTree = solution.makeSubTree(depthDist(randomEngine), randomEngine, &ExpressionTree::makeNodeByGrow);
	}

	//変異させる
	if (up == nullptr) {
		solution.roots[rootIndex] = subTree;
		solution.roots[rootIndex]->setParent(nullptr);
	} else {
		up->replaceChild(*target, subTree);
	}
}

ExpressionTree::ExpressionTree(const ProgramType& programType, int maxDepth) : Program(programType) {
	this->maxDepth = maxDepth;
}

ExpressionTree::ExpressionTree(const ExpressionTree& tree) : Program(tree.getProgramType()), maxDepth(tree.maxDepth) {
	for (shared_ptr<ExpressionNode> root : tree.roots) {
		roots.push_back(shared_ptr<ExpressionNode>(root->clone()));
	}
	nonterminals = tree.nonterminals;
	terminals = tree.terminals;
}

vector<double> ExpressionTree::operator()(const vector<double>& input) {
	assert((unsigned int)getProgramType().getOutputType().getSize() == roots.size());
	assert(getProgramType().getInputType().accepts(input));

	//式の実行
	vector<double> output(roots.size(), 0);
	for (unsigned int i = 0; i < roots.size(); ++i) {
		output[i] = roots[i]->operator()(input);
	}

	//出力のクリッピング
	getProgramType().getOutputType().clip(output);
	return output;
}

string ExpressionTree::toString() const {
	string ret;
	for (unsigned int i = 0; i < roots.size(); ++i) {
		ret += "tree" + to_string(i) + ": " + roots[i]->toString() + "\n";
	}
	return ret;
}

void ExpressionTree::crossover(const std::string& method, ExpressionTree& other, std::mt19937_64& randomEngine) {
	assert(this->roots.size() == other.roots.size());
	for (unsigned int i = 0; i < roots.size(); ++i) {
		ExpressionTree::normalCrossover(*this, other, i, randomEngine);
	}
}

void ExpressionTree::mutation(const std::string& method, std::mt19937_64& randomEngine) {
	for (unsigned int i = 0; i < roots.size(); ++i) {
		ExpressionTree::normalMutation(*this, i, randomEngine);
	}
}

void ExpressionTree::addOperator(shared_ptr<ExpressionNode> op) {
	if (op->getArity() == 0) {
		terminals.push_back(op);
	} else {
		nonterminals.push_back(op);
	}
}

void ExpressionTree::grow(mt19937_64& randomEngine) {
	roots = vector<shared_ptr<ExpressionNode>>(getProgramType().getOutputType().getSize(), nullptr);
	for (shared_ptr<ExpressionNode>& root : roots) {
		root = makeSubTree(maxDepth, randomEngine, &ExpressionTree::makeNodeByGrow);
	}
}

void ExpressionTree::full(mt19937_64& randomEngine) {
	roots = vector<shared_ptr<ExpressionNode>>(getProgramType().getOutputType().getSize(), nullptr);
	for (shared_ptr<ExpressionNode>& root : roots) {
		root = makeSubTree(maxDepth, randomEngine, &ExpressionTree::makeNodeByFull);
	}
}

shared_ptr<ExpressionNode> ExpressionTree::makeSubTree(int maxDepth, mt19937_64& randomEngine, MethodMakeNode method) {
	shared_ptr<ExpressionNode> rootNode = (this->*method)(0, maxDepth, randomEngine);
	rootNode->randomize(getProgramType(), randomEngine);

	vector<shared_ptr<ExpressionNode>> now;
	now.push_back(rootNode);

	for (int thisDepth = 1; thisDepth <= maxDepth; ++thisDepth) {
		vector<shared_ptr<ExpressionNode>> next;
		for (shared_ptr<ExpressionNode> node : now) {
			for (int i = 0; i < node->getArity(); ++i) {
				std::shared_ptr<ExpressionNode> child = (this->*method)(thisDepth, maxDepth, randomEngine);
				child->randomize(getProgramType(), randomEngine);
				node->addChild(child);
				next.push_back(child);
			}
		}
		now = next;
	}
	return rootNode;
}

shared_ptr<ExpressionNode> ExpressionTree::makeNodeByGrow(int thisDepth, int maxDepth, mt19937_64& randomEngine) {
	if (thisDepth == maxDepth) {
		uniform_int_distribution<int> dist(0, terminals.size() - 1);
		return shared_ptr<ExpressionNode>(terminals[dist(randomEngine)]->clone());
	}
	uniform_int_distribution<int> dist(0, nonterminals.size() + terminals.size() - 1);
	int select = dist(randomEngine);
	if ((unsigned int)select < nonterminals.size()) {
		return shared_ptr<ExpressionNode>(nonterminals[select]->clone());
	}
	return shared_ptr<ExpressionNode>(terminals[select - nonterminals.size()]->clone());
}

shared_ptr<ExpressionNode> ExpressionTree::makeNodeByFull(int thisDepth, int maxDepth, mt19937_64& randomEngine) {
	if (thisDepth == maxDepth) {
		uniform_int_distribution<int> dist(0, terminals.size() - 1);
		return shared_ptr<ExpressionNode>(terminals[dist(randomEngine)]->clone());
	}
	uniform_int_distribution<int> dist(0, nonterminals.size() - 1);
	return shared_ptr<ExpressionNode>(nonterminals[dist(randomEngine)]->clone());
}
