/*
 * Regression.cpp
 *
 *  Created on: 2013/06/15
 *      Author: kryozahiro
 */

#include "Regression.h"

#include <cassert>
#include <boost/lexical_cast.hpp>
using namespace std;

Regression::Regression(const DataSet& dataSet) : Problem(dataSet.getProgramType()), dataSet(dataSet) {
}

Regression::Regression(const boost::property_tree::ptree& gameTree, mt19937_64& randomEngine) :
		Problem(fromPtree(gameTree, randomEngine).getProgramType()),
		dataSet(fromPtree(gameTree, randomEngine)) {
}

string Regression::toString() const {
	return dataSet.toString();
}

double Regression::evaluate(Program& program) {
	double diff = 0;
	for (int i = 0; i < dataSet.getSize(); ++i) {
		vector<double> output = program(dataSet[i].first);
		assert(getProgramType().getOutputType().accepts(output));

		//二乗誤差を求める
		for (int k = 0; k < getProgramType().getOutputType().getSize(); ++k) {
			double error = output[k] - dataSet[i].second[k];
			//diff += error * error;
			diff += fabs(error);
		}
	}
	if (diff < 0) {
		diff = DBL_MAX;
	}
	return diff;
}

/*string Regression::showProgram(Program& program) const {
	string ret;
	for (int i = 0; i < dataSet.getSize(); ++i) {
		const pair<vector<double>, vector<double>>& point = dataSet[i];

		ret += "in = ( ";
		for (double inputElement : point.first) {
			ret += boost::lexical_cast<string>(inputElement) + " ";
		}
		ret += ")\tdiffout = ( ";
		vector<double> output = program(point.first);
		for (unsigned int i = 0; i < output.size(); ++i) {
			ret += boost::lexical_cast<string>(output[i] - point.second[i]) + " ";
		}
		ret += ")\n";
	}
	return ret;
}*/

DataSet Regression::fromPtree(const boost::property_tree::ptree& gameTree, std::mt19937_64& randomEngine) {
	boost::property_tree::ptree dataTree = gameTree.get_child("DataSet");
	return DataSet(dataTree, randomEngine);
}
