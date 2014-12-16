/*
 * NeuralLayer.h
 *
 *  Created on: 2013/11/07
 *      Author: kryozahiro
 */

#ifndef NEURALLAYER_H_
#define NEURALLAYER_H_

#include <random>
#include "../Program.h"

class NeuralLayer : public Program {
public:
	//伝達関数
	static double unitStep(double x);
	static double sigmoid(double a, double x);

	//通常の変異
	static void crossover(NeuralLayer& parent1, NeuralLayer& parent2, std::mt19937_64& randomEngine);
	static void normalMutation(NeuralLayer& layer, std::mt19937_64& randomEngine);

	NeuralLayer(int in, int out, std::function<double(double)> transfer);
	explicit NeuralLayer(std::istream& is);
	virtual ~NeuralLayer() = default;
	CPPUTIL_CLONEABLE;
	virtual std::vector<double> operator()(const std::vector<double>& input);

	//入出力
	virtual std::string toString() const;
	virtual void read(std::istream& is);
	virtual void write(std::ostream& os) const;

	template <class URNG>
	void resizeInput(int size, URNG& randomEngine) {
		std::uniform_real_distribution<double> dist(-0.1, 0.1);
		for (std::vector<double>& node : weights) {
			if ((unsigned int)1 + size <= node.size()) {
				node.resize(1 + size);
				continue;
			}

			std::vector<double> add(size - node.size(), 0);
			for (double& weight : add) {
				weight = dist(randomEngine);
			}
			node.insert(node.end(), add.begin(), add.end());
		}
		prevSize = 1 + size;
	}

	template <class URNG>
	void resizeOutput(int size, URNG& randomEngine) {
		std::uniform_real_distribution<double> dist(-0.1, 0.1);
		if ((unsigned int)size <= weights.size()) {
			weights.resize(size);
			return;
		}

		int addSize = size - weights.size();
		for (int i = 0; i < addSize; ++i) {
			std::vector<double> node(prevSize, 0);
			for (double& weight : node) {
				weight = dist(randomEngine);
			}
			weights.push_back(node);
		}
	}

	//ランダム値で初期化する
	template <class URNG>
	void randomize(URNG& randomEngine) {
		std::uniform_real_distribution<double> dist(-0.1, 0.1);
		for (unsigned int y = 0; y < weights.size(); ++y) {
			for (unsigned int x = 0; x < weights[y].size(); ++x) {
				weights[y][x] = dist(randomEngine);
			}
		}
	}

private:
	//前段のサイズ = 1 + 入力数
	int prevSize;

	//重み[出力数][1 + 入力数]
	//先頭は閾値
	std::vector<std::vector<double>> weights;

	std::function<double(double)> transfer;
};

#endif /* NEURALLAYER_H_ */
