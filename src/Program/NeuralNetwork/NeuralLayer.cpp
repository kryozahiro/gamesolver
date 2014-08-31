/*
 * NeuralLayer.cpp
 *
 *  Created on: 2013/11/07
 *      Author: kryozahiro
 */

#include <boost/lexical_cast.hpp>
#include "NeuralLayer.h"
using namespace std;

double NeuralLayer::unitStep(double x) {
	return (x >= 0) ? 1 : 0;
}

double NeuralLayer::sigmoid(double a, double x) {
	return 1.0 / (1.0 + exp(-a * x));
}

void NeuralLayer::crossover(NeuralLayer& parent1, NeuralLayer& parent2, std::mt19937_64& randomEngine) {
	for (unsigned int y = 0; y < parent1.weights.size(); ++y) {
		for (unsigned int x = 0; x < parent1.weights[y].size(); ++x) {
			uniform_real_distribution<double> weightDist(min(parent1.weights[y][x], parent2.weights[y][x]), max(parent1.weights[y][x], parent2.weights[y][x]));
			parent1.weights[y][x] = weightDist(randomEngine);
			parent2.weights[y][x] = weightDist(randomEngine);
		}
	}
}

void NeuralLayer::normalMutation(NeuralLayer& layer, std::mt19937_64& randomEngine) {
	uniform_real_distribution<double> mutateDist(0, 1);
	uniform_real_distribution<double> weightDist(-1, 1);
	for (unsigned int y = 0; y < layer.weights.size(); ++y) {
		for (unsigned int x = 0; x < layer.weights[y].size(); ++x) {
			if (mutateDist(randomEngine) > 0.05) {
				continue;
			}
			layer.weights[y][x] = 2.0 * weightDist(randomEngine);
			//layer.weights[y][x] += weightDist(randomEngine);
		}
	}
}

NeuralLayer::NeuralLayer(int in, int out, std::function<double(double)> transfer) {
	this->prevSize = 1 + in;
	this->weights = vector<vector<double>>(out, vector<double>(prevSize, 0));
	this->transfer = transfer;
}

NeuralLayer::NeuralLayer(istream& is) {
	is.ignore(std::numeric_limits<std::streamsize>::max(), '(');
	is >> prevSize;

	char c;
	is >> c;
	while (c != ')') {
		is.putback(c);
		is.ignore(std::numeric_limits<std::streamsize>::max(), '(');

		vector<double> node;
		is >> c;
		while (c != ')') {
			is.putback(c);
			double weight;
			is >> weight;
			node.push_back(weight);
			is >> c;
		}
		weights.push_back(node);

		is >> c;
	}
	//TODO transfer
	transfer = [&](double x) -> double { return NeuralLayer::sigmoid(1.0, x); };
}

Program* NeuralLayer::clone() {
	return new NeuralLayer(*this);
}

vector<double> NeuralLayer::operator()(const vector<double>& input) {
	assert(1 + input.size() == (unsigned int)prevSize);

	vector<double> output(weights.size(), 0);
	for (unsigned int y = 0; y < weights.size(); ++y) {
		//閾値
		output[y] += weights[y][0];
		//入力
		for (unsigned int x = 1; x < weights[y].size(); ++x) {
			output[y] += weights[y][x] * input[x - 1];
		}
		output[y] = transfer(output[y]);
	}
	return output;
}

string NeuralLayer::toString() const {
	string ret;
	for (unsigned int y = 0; y < weights.size(); ++y) {
		for (unsigned int x = 0; x < weights[y].size(); ++x) {
			ret += boost::lexical_cast<string>(weights[y][x]) + " ";
		}
		ret += "\n";
	}
	return ret;
}

void NeuralLayer::read(istream& is) {
	*this = NeuralLayer(is);
}

void NeuralLayer::write(ostream& os) const {
	os << "( " << prevSize << " ";
	for (const vector<double>& node : weights) {
		os << "(";
		for (double weight : node) {
			os << weight << " ";
		}
		os << ")";
	}
	os << ")";
}
