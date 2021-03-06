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
		optimum = tree.get<double>("Optimum");
	} catch (std::exception& e) {}
	try {
		minimumTime = tree.get<double>("MinimumTime");
	} catch (std::exception& e) {}
	try {
		maximumTime = tree.get<double>("MaximumTime");
	} catch (std::exception& e) {}
	try {
		minimumEvaluation = tree.get<int>("MinimumEvaluation");
	} catch (std::exception& e) {}
	try {
		maximumEvaluation = tree.get<int>("MaximumEvaluation");
	} catch (std::exception& e) {}
	try {
		bestImprovement = tree.get<double>("BestImprovement");
		assert(bestImprovement <= 0);
	} catch (std::exception& e) {}
	try {
		meanImprovement = tree.get<double>("MeanImprovement");
		assert(meanImprovement <= 0);
	} catch (std::exception& e) {}
	try {
		noBestImprovementScale = tree.get<double>("NoBestImprovementScale");
	} catch (std::exception& e) {}
	try {
		noMeanImprovementScale = tree.get<double>("NoMeanImprovementScale");
	} catch (std::exception& e) {}
}

bool TerminationCriteria::meets(int evaluation, SolutionHistory& solutionHistory) {
	if (optimum > -DBL_MAX and solutionHistory.getGeneration(-1).front()->getFitness() <= optimum) {
		return true;
	}
	if (minimumTime > 0 and solutionHistory.getElapsedTime() < minimumTime) {
		return false;
	}
	if (maximumTime > 0 and solutionHistory.getElapsedTime() >= maximumTime) {
		return true;
	}
	if (minimumEvaluation > 0 and evaluation < minimumEvaluation) {
		return false;
	}
	if (maximumEvaluation > 0 and evaluation >= maximumEvaluation) {
		return true;
	}

	//打ち切り
	if (bestImprovement == 0 and meanImprovement == 0 and noBestImprovementScale == 0 and noMeanImprovementScale == 0) {
		//打ち切り条件なし
		return false;
	}
	if (bestImprovement < 0 and solutionHistory.getLastGenerationNum() >= 5) {
		double now = solutionHistory.getGeneration(-1).front()->getFitness();
		double prev = solutionHistory.getGeneration(-6).front()->getFitness();
		if (now - prev < bestImprovement) {
			return false;
		}
	}
	if (meanImprovement < 0 and solutionHistory.getLastGenerationNum() >= 5) {
		double now = SolutionHistory::getMeanFitness(solutionHistory.getGeneration(-1));
		double prev = SolutionHistory::getMeanFitness(solutionHistory.getGeneration(-6));
		if (now - prev < meanImprovement) {
			return false;
		}
	}
	if (noBestImprovementScale > 0) {
		double now = solutionHistory.getGeneration(-1).front()->getFitness();
		if (best > now) {
			best = now;
			noBestImprovementCount = 0;
		} else {
			++noBestImprovementCount;
		}
		if ((double)noBestImprovementCount / (double)solutionHistory.getLastGenerationNum() < noBestImprovementScale) {
			return false;
		}
	}
	if (noMeanImprovementScale > 0) {
		double now = SolutionHistory::getMeanFitness(solutionHistory.getGeneration(-1));
		if (mean > now) {
			mean = now;
			noMeanImprovementCount = 0;
		} else {
			++noMeanImprovementCount;
		}
		if ((double)noMeanImprovementCount / (double)solutionHistory.getLastGenerationNum() < noMeanImprovementScale) {
			return false;
		}
	}
	return true;
}
