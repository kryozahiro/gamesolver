/*
 * DataSet.h
 *
 *  Created on: 2014/06/11
 *      Author: kryozahiro
 */

#ifndef DATASET_H_
#define DATASET_H_

#include <memory>
#include <vector>
#include <algorithm>
#include <random>
#include <boost/property_tree/ptree.hpp>
#include "../Program/ProgramType.h"

class DataSet {
public:
	static std::shared_ptr<DataSet> getNguyen(int i, std::mt19937_64& randomEngine);

	DataSet();
	DataSet(const ProgramType& programType);
	DataSet(const boost::property_tree::ptree& dataTree, std::mt19937_64& randomEngine);

	const ProgramType& getProgramType() const;
	int getSize() const;

	//データの取得
	std::pair<std::vector<double>, std::vector<double>>& operator[](int i);
	const std::pair<std::vector<double>, std::vector<double>>& operator[](int i) const;
	std::vector<double>& getInput(int i);
	std::vector<double>& getOutput(int i);

	//データの追加
	void addData(const std::vector<double>& input, const std::vector<double>& output);
	template <class URNG>
	void addData(URNG randomEngine, int sample, std::function<std::vector<double>(const std::vector<double>)> func) {
		for (int i = 0; i < sample; ++i) {
			const DataType& inputType = getProgramType().getInputType();
			std::uniform_real_distribution<double> dist(inputType.getMin(), inputType.getMax());

			std::vector<double> input(inputType.getSize());
			std::generate(input.begin(), input.end(), [&](){
				return dist(randomEngine);
			});

			std::vector<double> output = func(input);
			getProgramType().getOutputType().clip(output);
			addData(input, output);
		}
	}

	std::string toString() const;

private:
	ProgramType programType;
	std::vector<std::pair<std::vector<double>, std::vector<double>>> data;
};

#endif /* DATASET_H_ */
