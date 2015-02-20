/*
 * DataSet.cpp
 *
 *  Created on: 2014/06/11
 *      Author: kryozahiro
 */

#include "CppUtil/GenericIo.h"
#include "DataSet.h"
using namespace std;
using namespace cpputil;

std::vector<double> DataSet::sphere(const std::vector<double>& input) {
	vector<double> output(1, 0);
	for (unsigned int i = 1; i < input.size(); ++i) {
		output[0] += input[i] * input[i];
	}
	return output;
}

std::vector<double> DataSet::schwefel(const std::vector<double>& input) {
	vector<double> output(1, 418.9829 * input.size());
	for (unsigned int i = 1; i < input.size(); ++i) {
		output[0] -= input[i] * sin(sqrt(abs(input[i])));
	}
	return output;
}

std::vector<double> DataSet::nguyen(int dimension, const std::vector<double>& input) {
	assert(dimension > 0);
	vector<double> output(1, 0);
	for (int i = 1; i < dimension; ++i) {
		output[0] += pow(input[0], (double)i);
	}
	return output;
}

DataSet::DataSet() {
}

DataSet::DataSet(const ProgramType& programType) : programType(programType) {
}

DataSet::DataSet(const boost::property_tree::ptree& dataTree, mt19937_64& randomEngine) {
	string name = dataTree.get<string>("Name");
	int dimension = dataTree.get<int>("Dimension");
	int size = dataTree.get<int>("Size");
	if (name.find("Sphere") != string::npos) {
		programType = ProgramType(DataType(-5.12, 5.12, 0.001, dimension), DataType(DBL_MIN, DBL_MAX, 0.001, 1));
		addData(randomEngine, size, sphere);
	} else if(name.find("Schwefel") != string::npos) {
		programType = ProgramType(DataType(-500, 500, 0.001, dimension), DataType(DBL_MIN, DBL_MAX, 0.001, 1));
		addData(randomEngine, size, schwefel);
	} else if (name.find("Nguyen") != string::npos) {
		programType = ProgramType(DataType(-1, 1, 0.001, 1), DataType(-10, 10, 0.001, 1));
		addData(randomEngine, size, [=](const vector<double>& input) {
			return nguyen(dimension, input);
		});
	}
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
