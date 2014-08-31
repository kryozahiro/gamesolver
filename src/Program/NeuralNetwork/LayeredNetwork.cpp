/*
 * LayeredNetwork.cpp
 *
 *  Created on: 2014/01/13
 *      Author: kryozahiro
 */

#include <cassert>
#include <algorithm>
#include "../../Util/Scale.h"
#include "LayeredNetwork.h"
using namespace std;

void LayeredNetwork::crossover(LayeredNetwork& parent1, LayeredNetwork& parent2, std::mt19937_64& randomEngine) {
	uniform_int_distribution<int> crossDist(0, parent1.layers.size() - 1);
	int cross = crossDist(randomEngine);

	NeuralLayer::crossover(*parent1.layers[cross], *parent2.layers[cross], randomEngine);
	for (unsigned int i = cross + 1; i < parent1.layers.size(); ++i) {
		NeuralLayer tmp = *parent1.layers[i];
		*parent1.layers[i] = *parent2.layers[i];
		*parent2.layers[i] = tmp;
	}
}

void LayeredNetwork::normalMutation(LayeredNetwork& net, mt19937_64& randomEngine) {
	for (shared_ptr<NeuralLayer> layer : net.layers) {
		NeuralLayer::normalMutation(*layer, randomEngine);
	}
}

LayeredNetwork::LayeredNetwork(const ProgramType& programType, vector<int> nodes, function<double(double)> transfer) :
		programType(programType) {
	for (unsigned int i = 0; i < nodes.size() - 1; ++i) {
		shared_ptr<NeuralLayer> layer = make_shared<NeuralLayer>(nodes[i], nodes[i + 1], transfer);
		layers.push_back(layer);
	}
}

LayeredNetwork::LayeredNetwork(const LayeredNetwork& net) : programType(net.programType) {
	for (unsigned int i = 0; i < net.layers.size(); ++i) {
		shared_ptr<NeuralLayer> clone = make_shared<NeuralLayer>(*net.layers[i]);
		layers.push_back(clone);
	}
}

Program* LayeredNetwork::clone() {
	LayeredNetwork* newNet = new LayeredNetwork(*this);
	return newNet;
}

vector<double> LayeredNetwork::operator()(const vector<double>& input) {
	assert(programType.acceptsInput(input));

	vector<double> buf = input;
	for (shared_ptr<NeuralLayer> layer : layers) {
		buf = layer->operator()(buf);
	}

	const ParameterType& outputType = programType.getOutputType();
	transform(buf.begin(), buf.end(), buf.begin(), Scale::linearScaler(pair<double, double>(0, 1), pair<double, double>(outputType.getMin(), outputType.getMax())));
	return programType.clipOutput(buf);
}

string LayeredNetwork::toString() const {
	string ret;
	for (shared_ptr<NeuralLayer> layer : layers) {
		ret += layer->toString();
	}
	return ret;
}

void LayeredNetwork::read(istream& is) {
	is.ignore(std::numeric_limits<std::streamsize>::max(), '(');
	char c;
	is >> c;
	while (c != ')') {
		is.putback(c);
		layers.push_back(make_shared<NeuralLayer>(is));
		is >> c;
	}
}

void LayeredNetwork::write(ostream& os) const {
	os << "(";
	for (shared_ptr<NeuralLayer> layer : layers) {
		os << *layer << " ";
	}
	os << ")";
}

void LayeredNetwork::addLayer(shared_ptr<NeuralLayer> layer) {
	layers.push_back(layer);
}
