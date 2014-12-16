/*
 * LayeredNetwork.h
 *
 *  Created on: 2014/01/13
 *      Author: kryozahiro
 */

#ifndef LAYEREDNETWORK_H_
#define LAYEREDNETWORK_H_

#include <memory>
#include "../Program.h"
#include "NeuralLayer.h"

class LayeredNetwork : public Program {
public:
	static void crossover(LayeredNetwork& parent1, LayeredNetwork& parent2, std::mt19937_64& randomEngine);
	static void normalMutation(LayeredNetwork& net, std::mt19937_64& randomEngine);

	LayeredNetwork() = default;
	LayeredNetwork(const ProgramType& programType, std::vector<int> nodes, std::function<double(double)> transfer);
	LayeredNetwork(const LayeredNetwork& net);
	virtual ~LayeredNetwork() = default;
	CPPUTIL_CLONEABLE;
	virtual std::vector<double> operator()(const std::vector<double>& input);

	//入出力
	virtual std::string toString() const;
	virtual void read(std::istream& is);
	virtual void write(std::ostream& os) const;

	void addLayer(std::shared_ptr<NeuralLayer> layer);

	template <class URNG>
	void resizeLayers(std::vector<int> nodes, URNG& randomEngine) {
		assert(nodes.size() == layers.size() + 1);
		for (unsigned int i = 0; i < nodes.size() - 1; ++i) {
			layers[i]->resizeInput(nodes[i], randomEngine);
			layers[i]->resizeOutput(nodes[i + 1], randomEngine);
		}
	}

	template <class URNG>
	void randomize(URNG& randomEngine) {
		for (std::shared_ptr<NeuralLayer> layer : layers) {
			layer->randomize(randomEngine);
		}
	}

private:
	ProgramType programType;
	std::vector<std::shared_ptr<NeuralLayer>> layers;
};

#endif /* LAYEREDNETWORK_H_ */
