/*
 * ExpressionNode.cpp
 *
 *  Created on: 2013/06/19
 *      Author: kryozahiro
 */

#include <cassert>
#include <algorithm>
#include "ExpressionNode.h"
using namespace std;

vector<shared_ptr<ExpressionNode>> ExpressionNode::flatten(shared_ptr<ExpressionNode> root) {
	vector<shared_ptr<ExpressionNode>> flat;
	flat.push_back(root);
	for (shared_ptr<ExpressionNode> child : root->children) {
		vector<shared_ptr<ExpressionNode>> flattenChild = flatten(child);
		flat.insert(flat.end(), flattenChild.begin(), flattenChild.end());
	}
	return flat;
}

ExpressionNode::ExpressionNode(const ExpressionNode& node) {
	*this = node;
}

ExpressionNode& ExpressionNode::operator=(const ExpressionNode& node) {
	parent = node.parent;
	children.resize(node.children.size());
	transform(node.children.begin(), node.children.end(), children.begin(), [&](shared_ptr<ExpressionNode> child) {
		shared_ptr<ExpressionNode> cloneChild(child->clone());
		cloneChild->parent = this;
		return cloneChild;
	});
	return *this;
}

ExpressionNode* ExpressionNode::getParent() {
	return parent;
}

void ExpressionNode::setParent(ExpressionNode* parent) {
	this->parent = parent;
}

void ExpressionNode::addChild(shared_ptr<ExpressionNode> child) {
	child->setParent(this);
	children.push_back(child);
	assert((int)children.size() <= getArity());
}

void ExpressionNode::replaceChild(ExpressionNode& oldChild, shared_ptr<ExpressionNode> newChild) {
	for (shared_ptr<ExpressionNode>& child : children) {
		if (child.get() == &oldChild) {
			child = newChild;
			child->parent = this;
			return;
		}
	}
	assert(false);
}

int ExpressionNode::getDepth() const {
	if (parent == nullptr) {
		return 0;
	}
	return parent->getDepth() + 1;
}

int ExpressionNode::getHeight() const {
	if (children.size() == 0) {
		return 0;
	}
	int maxDepth = 0;
	for (shared_ptr<ExpressionNode> child : children) {
		int depth = child->getHeight();
		if (maxDepth < depth) {
			maxDepth = depth;
		}
	}
	return maxDepth + 1;
}

string ExpressionNode::toString() const {
	string ret;
	if (children.size() > 0) {
		ret += "(";
	}
	ret += getName();
	for (shared_ptr<ExpressionNode> child : children) {
		ret += " " + child->toString();
	}
	if (children.size() > 0) {
		ret += ")";
	}
	return ret;
}
