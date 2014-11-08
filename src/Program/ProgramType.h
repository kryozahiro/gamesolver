/*
 * ProgramType.h
 *
 *  Created on: 2013/11/09
 *      Author: kryozahiro
 */

#ifndef PROGRAMTYPE_H_
#define PROGRAMTYPE_H_

#include <vector>
#include "ParameterType.h"

class ProgramType : public cpputil::ReadWritable {
public:
	ProgramType() = default;
	ProgramType(const ParameterType& inputType, int inputSize, const ParameterType& outputType, int outputSize);
	virtual ~ProgramType() = default;

	//ReadWritableの実装
	virtual void read(std::istream& is) override;
	virtual void write(std::ostream& os) const override;

	//入出力型の取得
	const ParameterType& getInputType() const;
	int getInputSize() const;
	const ParameterType& getOutputType() const;
	int getOutputSize() const;

	std::vector<double> clipInput(const std::vector<double>& input) const;
	bool acceptsInput(const std::vector<double>& input) const;

	std::vector<double> clipOutput(const std::vector<double>& output) const;
	bool acceptsOutput(const std::vector<double>& output) const;

private:
	ParameterType inputType;
	int inputSize = 0;
	ParameterType outputType;
	int outputSize = 0;
};

#endif /* PROGRAMTYPE_H_ */
