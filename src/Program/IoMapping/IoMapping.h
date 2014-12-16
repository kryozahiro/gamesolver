/*
 * IoMapping.h
 *
 *  Created on: 2013/06/18
 *      Author: kryozahiro
 */

#ifndef IOMAPPING_H_
#define IOMAPPING_H_

#include <random>
#include "../Program.h"
class Problem;

class IoMapping : public Program {
public:
	IoMapping(const ProgramType& programType);
	virtual ~IoMapping() = default;
	CPPUTIL_CLONEABLE;
	virtual std::vector<double> operator()(const std::vector<double>& input);
	virtual std::string toString() const;

	//ランダム値で初期化する
	template <class URNG>
	void randomize(URNG& randomEngine) {
		const ParameterType outputType = programType.getOutputType();
		std::uniform_real_distribution<double> dist(outputType.getMin(), outputType.getMax());

		for (std::vector<double>& output : mapping) {
			for (double& outputElement : output) {
				outputElement = dist(randomEngine);
			}
		}
	}

protected:
	const ProgramType programType;
	int inputVariety;
	int outputVariety;

	//入力それぞれに対する出力
	std::vector<std::vector<double>> mapping;
};

#endif /* IOMAPPING_H_ */
