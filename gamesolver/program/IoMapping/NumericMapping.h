/*
 * NumericMapping.h
 *
 *  Created on: 2013/06/18
 *      Author: kryozahiro
 */

#ifndef NUMERICMAPPING_H_
#define NUMERICMAPPING_H_

class NumericMapping : public IoMapping {
public:
	virtual ~NumericMapping() = default;

	//交叉
	//TODO: BLX-a
	/*template <class URNG>
	static void crossover(NumericMapping& parent1, NumericMapping& parent2, URNG& randomEngine) {
		int domain = parent1.mapping.size();
		int outputLength = parent1.mapping[0].size();
		std::uniform_int_distribution<int> dist(0, domain * outputLength);
		int start = dist(randomEngine);

		for (int i = 0; i < domain; ++i) {
			for (int k = 0; k < outputLength; ++k) {
				if (i * outputLength + k >= start) {
					std::swap(parent1.mapping[i][k], parent2.mapping[i][k]);
				}
			}
		}
	}

	//変異
	template <class URNG>
	static void mutation(NumereicMapping& solution, URNG& randomEngine) {
		std::uniform_int_distribution<int> mutationDist(0, 1000);
		std::uniform_int_distribution<int> valueDist(solution.outputMinMax.first, solution.outputMinMax.second);
		for (std::vector<int>& output : solution.mapping) {
			for (int& outputElement : output) {
				if (mutationDist(randomEngine) == 0) {
					outputElement = valueDist(randomEngine);
				}
			}
		}
	}*/
};

#endif /* NUMERICMAPPING_H_ */
