/*
 * TerminationCriteria.cpp
 *
 *  Created on: 2015/02/19
 *      Author: kryozahiro
 */

#include "TerminationCriteria.h"
using namespace std;

TerminationCriteria::TerminationCriteria(boost::property_tree::ptree& tree) {
	try {
		minimumEvaluation = tree.get<int>("MinimumEvaluation");
	} catch (std::exception& e) {}
	try {
		elapsedTime = tree.get<double>("ElapsedTime");
	} catch (std::exception& e) {}
	try {
		evaluationCount = tree.get<int>("EvaluationCount");
	} catch (std::exception& e) {}
	try {
		improvement = tree.get<double>("Improvement");
	} catch (std::exception& e) {}
	try {
		noImprovementScale = tree.get<double>("NoImprovementScale");
	} catch (std::exception& e) {}
}

bool TerminationCriteria::meets(int evaluation, SolutionHistory& solutionHistory) {
	if (minimumEvaluation > 0 and evaluation < minimumEvaluation) {
		return false;
	}
	if (elapsedTime > 0 and solutionHistory.getElapsedTime() > elapsedTime) {
		return true;
	}
	if (evaluationCount > 0 and evaluation >= evaluationCount) {
		return true;
	}
	if (improvement > 0 and solutionHistory.getLastGeneration() >= 1) {
		double latest = solutionHistory.getPopulation(-1).front()->getFitness();
		double now = solutionHistory.getPopulation(-2).front()->getFitness();
		if (latest - now < improvement) {
			return true;
		}
	}
	if (noImprovementScale > 0) {
		if (best > solutionHistory.getPopulation(-1).front()->getFitness()) {
			best = solutionHistory.getPopulation(-1).front()->getFitness();
			count = 0;
		} else {
			++count;
		}
		if ((double)count / (double)solutionHistory.getLastGeneration() > noImprovementScale) {
			return true;
		}
	}
	return false;
}
