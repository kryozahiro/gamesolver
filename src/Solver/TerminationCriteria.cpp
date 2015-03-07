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
		minimumTime = tree.get<double>("MinimumTime");
	} catch (std::exception& e) {}
	try {
		elapsedTime = tree.get<double>("ElapsedTime");
	} catch (std::exception& e) {}
	try {
		minimumEvaluation = tree.get<int>("MinimumEvaluation");
	} catch (std::exception& e) {}
	try {
		evaluationCount = tree.get<int>("EvaluationCount");
	} catch (std::exception& e) {}
	try {
		bestImprovement = tree.get<double>("BestImprovement");
	} catch (std::exception& e) {}
	try {
		meanImprovement = tree.get<double>("MeanImprovement");
	} catch (std::exception& e) {}
	try {
		noBestImprovementScale = tree.get<double>("NoBestImprovementScale");
	} catch (std::exception& e) {}
	try {
		noMeanImprovementScale = tree.get<double>("NoMeanImprovementScale");
	} catch (std::exception& e) {}
}

bool TerminationCriteria::meets(int evaluation, SolutionHistory& solutionHistory) {
	if (minimumTime > 0 and solutionHistory.getElapsedTime() < minimumTime) {
		return false;
	}
	if (elapsedTime > 0 and solutionHistory.getElapsedTime() >= elapsedTime) {
		return true;
	}
	if (minimumEvaluation > 0 and evaluation < minimumEvaluation) {
		return false;
	}
	if (evaluationCount > 0 and evaluation >= evaluationCount) {
		return true;
	}
	if (bestImprovement > 0 and solutionHistory.getLastGeneration() >= 5) {
		double now = solutionHistory.getPopulation(-1).front()->getFitness();
		double prev = solutionHistory.getPopulation(-6).front()->getFitness();
		if (abs(now - prev) < bestImprovement) {
			return true;
		}
	}
	if (meanImprovement > 0 and solutionHistory.getLastGeneration() >= 5) {
		double now = SolutionHistory::getMeanFitness(solutionHistory.getPopulation(-1));
		double prev = SolutionHistory::getMeanFitness(solutionHistory.getPopulation(-6));
		if (abs(now - prev) < meanImprovement) {
			return true;
		}
	}
	if (noBestImprovementScale > 0) {
		double now = solutionHistory.getPopulation(-1).front()->getFitness();
		if (best > now) {
			best = now;
			noBestImprovementCount = 0;
		} else {
			++noBestImprovementCount;
		}
		if ((double)noBestImprovementCount / (double)solutionHistory.getLastGeneration() > noBestImprovementScale) {
			return true;
		}
	}
	if (noMeanImprovementScale > 0) {
		double now = SolutionHistory::getMeanFitness(solutionHistory.getPopulation(-1));
		if (mean > now) {
			mean = now;
			noMeanImprovementCount = 0;
		} else {
			++noMeanImprovementCount;
		}
		if ((double)noMeanImprovementCount / (double)solutionHistory.getLastGeneration() > noMeanImprovementScale) {
			return true;
		}
	}
	return false;
}
