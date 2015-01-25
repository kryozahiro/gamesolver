/*
 * DataSet.cpp
 *
 *  Created on: 2014/06/11
 *      Author: kryozahiro
 */

#include "CppUtil/GenericIo.h"
#include "DataSet.h"
using namespace std;

shared_ptr<DataSet> DataSet::getNguyen(int i, mt19937_64& randomEngine) {
	shared_ptr<DataSet> dataSet;
	if (0 < i and i <= 4) {
		ProgramType programType(DataType(-1, 1, 0.001, 1), DataType(-10, 10, 0.001, 1));
		dataSet = make_shared<DataSet>(programType);
		dataSet->addData(randomEngine, 20, [&](const vector<double>& input){
			double x = input[0];
			double y = 0;
			for (int k = 1; k < 3 + i; ++k) {
				y += pow(x, (double)k);
			}
			return vector<double>(1, y);
		});
	} else {
		assert(false);
	}
	return dataSet;
}

DataSet::DataSet() {
}

DataSet::DataSet(const ProgramType& programType) : programType(programType) {
}

DataSet::DataSet(const boost::property_tree::ptree& dataTree, mt19937_64& randomEngine) {
	string name = dataTree.data();
	*this = *getNguyen(name.back() - '0', randomEngine);
}

const ProgramType& DataSet::getProgramType() const {
	return programType;
}

int DataSet::getSize() const {
	return data.size();
}

pair<vector<double>, vector<double>>& DataSet::operator[](int i) {
	return data[i];
}

const pair<vector<double>, vector<double>>& DataSet::operator[](int i) const {
	return data[i];
}

vector<double>& DataSet::getInput(int i) {
	return data[i].first;
}

vector<double>& DataSet::getOutput(int i) {
	return data[i].second;
}

void DataSet::addData(const vector<double>& input, const vector<double>& output) {
	data.push_back(make_pair(input, output));
}

string DataSet::toString() const {
	string ret = "DataSet\n";
	for (const pair<vector<double>, vector<double>>& point : data) {
		ret += boost::lexical_cast<string>(point) + "\n";
	}
	return ret;
}
