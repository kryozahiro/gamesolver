/*
 * Solution.cpp
 *
 *  Created on: 2014/07/23
 *      Author: kryozahiro
 */

#include <boost/lexical_cast.hpp>
#include "Solution.h"
using namespace std;

Solution::Solution(std::shared_ptr<Program> program) : program(program) {
}

Solution::Solution(const Solution& solution) :
		program(solution.program->clone()),
		fitness(solution.fitness),
		generation(solution.generation),
		children(solution.children),
		time(solution.time) {
}

std::shared_ptr<Program> Solution::getProgram() {
	return program;
}

vector<double> Solution::operator()(const vector<double>& input) {
	return program->operator()(input);
}

string Solution::toString() const {
	string ret = "generation:" + boost::lexical_cast<string>(generation) +
			" time:" + boost::lexical_cast<string>(time) +
			" fitness:" + boost::lexical_cast<string>(fitness) +
			" this:" + boost::lexical_cast<string>(reinterpret_cast<long long>(this)) + "\n";

	ret += "children: ";
	if (children.size() == 0) {
		ret += "none";
	}
	for (const shared_ptr<Solution>& child : children) {
		ret += boost::lexical_cast<string>(reinterpret_cast<long long>(child.get())) + ", ";
	}
	ret += "\n";
	ret += program->toString();
	return ret;
}

string Solution::showRelation() const {
	if (children.size() == 0) {
		return boost::lexical_cast<string>(generation) + " " + boost::lexical_cast<string>(fitness) + "\n\n";
	}

	string ret;
	for (const shared_ptr<Solution>& child : children) {
		ret += boost::lexical_cast<string>(generation) + " " + boost::lexical_cast<string>(fitness) + "\n";
		ret += boost::lexical_cast<string>(child->generation) + " " + boost::lexical_cast<string>(child->fitness) + "\n";
		ret += "\n";
	}
	return ret;
}

void Solution::setFitness(double fitness) {
	this->fitness = fitness;
}

double Solution::getFitness() const {
	return fitness;
}

void Solution::setGeneration(int generation) {
	this->generation = generation;
}

int Solution::getGeneration() const {
	return generation;
}

std::shared_ptr<Solution> Solution::createChild(bool resetFitness) {
	std::shared_ptr<Solution> child = std::make_shared<Solution>(*this);
	if (resetFitness) {
		child->setFitness(DBL_MAX);
	}
	child->setGeneration(this->getGeneration() + 1);
	child->children.clear();
	child->setTime(0);
	this->addChild(child);
	return child;
}

void Solution::addChild(std::shared_ptr<Solution>& child) {
	children.push_back(child);
}

void Solution::setTime(int time) {
	this->time = time;
}
