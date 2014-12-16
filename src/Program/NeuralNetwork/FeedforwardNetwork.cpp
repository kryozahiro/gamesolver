/*
 * FeedforwardNetwork.cpp
 *
 *  Created on: 2014/01/13
 *      Author: kryozahiro
 */

#include <cassert>
#include <cmath>
#include <boost/algorithm/string.hpp>
#include "CppUtil/GenericIo.h"
#include "../../Util/fastonebigheader.h"
#include "FeedforwardNetwork.h"
using namespace std;
namespace pt = boost::property_tree;

void FeedforwardNetwork::crossover(FeedforwardNetwork& parent1, FeedforwardNetwork& parent2, mt19937_64& randomEngine) {
	double alpha = 0.1;
	for (unsigned int y = 0; y < parent1.weights.size(); ++y) {
		for (unsigned int x = y + 1; x < parent1.weights[y].size(); ++x) {
			if (parent1.changeable[y][x]) {
				pair<double, double> mm = minmax(parent1.weights[y][x], parent2.weights[y][x]);
				double dist = mm.second - mm.first;
				uniform_real_distribution<double> weightDist(mm.first - alpha * dist, mm.second + alpha * dist);
				parent1.weights[y][x] = weightDist(randomEngine);
				parent2.weights[y][x] = weightDist(randomEngine);
				parent1.weights[y][x] = max(-10.0, min(parent1.weights[y][x], 10.0));
				parent2.weights[y][x] = max(-10.0, min(parent2.weights[y][x], 10.0));
			}
		}
	}
}

void FeedforwardNetwork::normalMutation(FeedforwardNetwork& net, mt19937_64& randomEngine) {
	uniform_real_distribution<double> mutateDist(0, 1);
	uniform_real_distribution<double> weightDist(0.9, 1.1);
	for (unsigned int y = 0; y < net.weights.size(); ++y) {
		for (unsigned int x = y + 1; x < net.weights[y].size(); ++x) {
			if (net.changeable[y][x] and mutateDist(randomEngine) < 0.1) {
				net.weights[y][x] = net.weights[y][x] * weightDist(randomEngine);
				net.weights[y][x] = max(-10.0, min(net.weights[y][x], 10.0));
			}
		}
	}
}

double FeedforwardNetwork::unitStep(double x) {
	return (x >= 0) ? 1 : 0;
}

double FeedforwardNetwork::sigmoid(double a, double x) {
	//return 1.0 / (1.0 + exp(-a * x));
	return 1.0 / (1.0 + fasterexp(-a * x));
}

FeedforwardNetwork::FeedforwardNetwork(const ProgramType& programType, int hiddenSize, function<double(double)> activation)
		: programType(programType) {
	int nodeSize = 1 + programType.getInputSize() + hiddenSize + programType.getOutputSize();
	this->adjacency = boost::container::vector<boost::container::vector<bool>>(nodeSize, boost::container::vector<bool>(nodeSize, false));
	this->weights = vector<vector<double>>(nodeSize, vector<double>(nodeSize, 0));
	this->activation = activation;
	this->changeable = boost::container::vector<boost::container::vector<bool>>(nodeSize, boost::container::vector<bool>(nodeSize, false));
}

FeedforwardNetwork::FeedforwardNetwork(const ProgramType& programType, const boost::property_tree::ptree& node, std::mt19937_64& randomEngine)
		: programType(programType) {
	//活性化関数の初期化
	string activationName = node.get<string>("Activation");
	if (activationName == "Unit") {
		activation = FeedforwardNetwork::unitStep;
	} else if (activationName == "Sigmoid") {
		activation = [](double x) {
			return FeedforwardNetwork::sigmoid(1.0, x);
		};
	}

	//行列の生成
	int hiddenSize = node.get<int>("Hidden");
	int nodeSize = 1 + programType.getInputSize() + hiddenSize + programType.getOutputSize();
	this->adjacency = boost::container::vector<boost::container::vector<bool>>(nodeSize, boost::container::vector<bool>(nodeSize, false));
	this->weights = vector<vector<double>>(nodeSize, vector<double>(nodeSize, 0));
	this->changeable = boost::container::vector<boost::container::vector<bool>>(nodeSize, boost::container::vector<bool>(nodeSize, false));

	//接続の生成
	const pt::ptree& adjacencyTree = node.get_child("Adjacency");
	for (const pt::ptree::value_type& kvp : adjacencyTree) {
		if (kvp.first != "Connection") {
			assert(false);
		}

		int src0 = readConnectionNotation(kvp.second.get<string>("<xmlattr>.src0"));
		int src1 = readConnectionNotation(kvp.second.get<string>("<xmlattr>.src1"));
		int dest0 = readConnectionNotation(kvp.second.get<string>("<xmlattr>.dest0"));
		int dest1 = readConnectionNotation(kvp.second.get<string>("<xmlattr>.dest1"));
		setAdjacency(src0, src1, dest0, dest1, true);
		setChangeable(src0, src1, dest0, dest1, true);
	}

	//重みの初期化
	randomizeWeights(randomEngine);
}

vector<double> FeedforwardNetwork::operator()(const vector<double>& input) {
	assert(programType.acceptsInput(input));
	vector<double> node(weights.size());
	for (unsigned int y = 0; y < weights.size(); ++y) {
		if (y == 0) {
			node[y] = 1;
		} else if (y <= (unsigned int)programType.getInputSize()) {
			node[y] = input[y - 1];
		} else {
			node[y] = activation(node[y]);
		}
		for (unsigned int x = y + 1; x < weights[y].size(); ++x) {
			if (adjacency[y][x]) {
				node[x] += weights[y][x] * node[y];
			}
		}
	}
	vector<double> output(node.end() - programType.getOutputSize(), node.end());
	return programType.clipOutput(output);
}

string FeedforwardNetwork::toString() const {
	return "";
}

void FeedforwardNetwork::read(istream& is) {
	tuple<
		ProgramType,
		boost::container::vector<boost::container::vector<bool>>,
		vector<vector<double>>,
		boost::container::vector<boost::container::vector<bool>>
	> t;
	is >> t;
	programType = get<0>(t);
	adjacency = get<1>(t);
	weights = get<2>(t);
	changeable = get<3>(t);
	assert(adjacency.size() == weights.size());
	assert(changeable.size() == weights.size());
	//TODO: activation
}

void FeedforwardNetwork::write(ostream& os) const {
	tuple<
		ProgramType,
		boost::container::vector<boost::container::vector<bool>>,
		vector<vector<double>>,
		boost::container::vector<boost::container::vector<bool>>
	> t(programType, adjacency, weights, changeable);
	os << t;
}

void FeedforwardNetwork::crossover(const std::string& method, FeedforwardNetwork& other, std::mt19937_64& randomEngine) {
	FeedforwardNetwork::crossover(*this, other, randomEngine);
}

void FeedforwardNetwork::mutation(const std::string& method, std::mt19937_64& randomEngine) {
	FeedforwardNetwork::normalMutation(*this, randomEngine);
}

int FeedforwardNetwork::getBias() {
	return 0;
}
int FeedforwardNetwork::getInputBegin() {
	return 1;
}
int FeedforwardNetwork::getInputEnd() {
	return 1 + programType.getInputSize();
}
int FeedforwardNetwork::getHiddenBegin() {
	return 1 + programType.getInputSize();
}
int FeedforwardNetwork::getHiddenEnd() {
	return adjacency.size() - programType.getOutputSize();
}
int FeedforwardNetwork::getOutputBegin() {
	return adjacency.size() - programType.getOutputSize();
}
int FeedforwardNetwork::getOutputEnd() {
	return adjacency.size();
}

void FeedforwardNetwork::setAdjacency(int from, int to, bool isAdjacent) {
	assert(0 <= from and from < (int)adjacency.size());
	assert(0 <= to and to < (int)adjacency.size());
	adjacency[from][to] = isAdjacent;
	adjacency[to][from] = isAdjacent;
}

void FeedforwardNetwork::setAdjacency(int first1, int last1, int first2, int last2, bool isAdjacent) {
	for (int x = first1; x < last1; ++x) {
		for (int y = first2; y < last2; ++y) {
			setAdjacency(x, y, isAdjacent);
		}
	}
}

void FeedforwardNetwork::setChangeable(int from, int to, bool isChangeable) {
	assert(0 <= from and from < (int)changeable.size());
	assert(0 <= to and to < (int)changeable.size());
	changeable[from][to] = isChangeable;
	changeable[to][from] = isChangeable;
}

void FeedforwardNetwork::setChangeable(int first1, int last1, int first2, int last2, bool isChangeable) {
	for (int x = first1; x < last1; ++x) {
		for (int y = first2; y < last2; ++y) {
			setChangeable(x, y, isChangeable);
		}
	}
}

void FeedforwardNetwork::insertNode(int pos, int size, bool addToBegin) {
	assert(0 <= pos and pos <= (int)adjacency.size());
	if (pos < getInputEnd() or (!addToBegin and pos == getInputEnd())) {
		programType = ProgramType(programType.getInputType(), programType.getInputSize() + size, programType.getOutputType(), programType.getOutputSize());
	} else if ((addToBegin and pos == getHiddenEnd()) or getHiddenEnd() < pos) {
		programType = ProgramType(programType.getInputType(), programType.getInputSize(), programType.getOutputType(), programType.getOutputSize() + size);
	}
	for (unsigned int y = 0; y < weights.size(); ++y) {
		adjacency[y].insert(adjacency[y].begin() + pos, size, false);
		weights[y].insert(weights[y].begin() + pos, size, 0);
		changeable[y].insert(changeable[y].begin() + pos, size, false);
	}
	adjacency.insert(adjacency.begin() + pos, size, boost::container::vector<bool>(adjacency.size() + size, false));
	weights.insert(weights.begin() + pos, size, vector<double>(weights.size() + size, 0));
	changeable.insert(changeable.begin() + pos, size, boost::container::vector<bool>(changeable.size() + size, false));
}

int FeedforwardNetwork::readConnectionNotation(const std::string& position) {
	string lower = boost::to_lower_copy(position);
	vector<string> splitted;
	boost::algorithm::split(splitted, lower, boost::algorithm::is_any_of("_"));

	if (splitted.size() < 2) {
		assert(false);
	}

	bool isIndex = isdigit(splitted[1][0]);
	int index = isIndex ? atoi(splitted[1].c_str()) : 0;

	if (splitted[0][0] == 'b') {
		return isIndex ? getBias() + index : getBias() + 1;
	} else if (splitted[0][0] == 'i') {
		return isIndex ? getInputBegin() + index : getInputEnd();
	} else if (splitted[0][0] == 'h') {
		return isIndex ? getHiddenBegin() + index : getHiddenEnd();
	} else if (splitted[0][0] == 'o') {
		return isIndex ? getOutputBegin() + index : getOutputEnd();
	} else {
		assert(false);
	}
	return 0;
}
