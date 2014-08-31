/*
 * GrayCodeMapping.cpp
 *
 *  Created on: 2013/06/18
 *      Author: kryozahiro
 */

#include "GrayCodeMapping.h"
using namespace std;

void GrayCodeMapping::onePointCrossover(GrayCodeMapping& parent1, GrayCodeMapping& parent2, std::mt19937_64& randomEngine) {
	int domain = parent1.mapping.size();
	int outputLength = parent1.mapping[0].size();
	int elementBits = ceil(log2(parent1.outputVariety));

	//交叉の開始位置
	std::uniform_int_distribution<int> dist(0, domain * outputLength * elementBits - 1);
	int startPos = dist(randomEngine);
	int startOutput = startPos / (outputLength * elementBits);
	int startElement = startPos % (outputLength * elementBits) / elementBits;
	int startBit = startPos % elementBits;

	//bitの交叉
	//std::cout << parent1.outputMinMax.second - parent1.outputMinMax.first + 1 << " ";
	//std::cout << elementBits << " " << startBit << " ";
	//std::cout << std::bitset<sizeof(int)*8>(parent1.mapping[startOutput][startElement]) << " ";
	//std::cout << std::bitset<sizeof(int)*8>(parent2.mapping[startOutput][startElement]) << " ";

	int mask = (~0) << startBit;
	int parent1Bits = encode(parent1.mapping[startOutput][startElement]);
	int parent2Bits = encode(parent2.mapping[startOutput][startElement]);
	int parent1New = decode((parent1Bits & mask) | (parent2Bits & ~mask));
	int parent2New = decode((parent2Bits & mask) | (parent1Bits & ~mask));
	parent1.mapping[startOutput][startElement] = parent1.programType.getOutputType().clip(parent1New);
	parent2.mapping[startOutput][startElement] = parent2.programType.getOutputType().clip(parent2New);

	//std::cout << std::bitset<sizeof(int)*8>(parent1.mapping[startOutput][startElement]) << " ";
	//std::cout << std::bitset<sizeof(int)*8>(parent2.mapping[startOutput][startElement]) << std::endl;

	//intのswap
	for (int i = startElement + 1; i < outputLength; ++i) {
		std::swap(parent1.mapping[startOutput][i], parent2.mapping[startOutput][i]);
	}
	for (int i = startOutput + 1; i < domain; ++i) {
		for (int k = 0; k < outputLength; ++k) {
			std::swap(parent1.mapping[i][k], parent2.mapping[i][k]);
		}
	}
}

void GrayCodeMapping::normalMutation(GrayCodeMapping& solution, std::mt19937_64& randomEngine) {
	uniform_int_distribution<int> mutationDist(0, 100);
	for (vector<double>& output : solution.mapping) {
		for (double& outputElement : output) {
			//std::cout << std::bitset<sizeof(int)*8>(outputElement) << " ";
			for (int flip = 1; flip != 0; flip <<= 1) {
				if (mutationDist(randomEngine) != 0) {
					continue;
				}
				int cand = decode(encode(outputElement) ^ flip);
				if (cand < solution.programType.getOutputType().getMin() or solution.programType.getOutputType().getMax() < cand) {
					continue;
				}
				outputElement = cand;
			}
			//std::cout << std::bitset<sizeof(int)*8>(outputElement) << std::endl;
		}
	}
}

int GrayCodeMapping::encode(int raw) {
	return raw ^ (raw >> 1);
}

int GrayCodeMapping::decode(int gray) {
	int raw = 0;
	while (gray > 0) {
		raw ^= gray;
		gray >>= 1;
	}
	return raw;
}
