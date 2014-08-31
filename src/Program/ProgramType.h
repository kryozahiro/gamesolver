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

class ProgramType : public ReadWritable {
public:
	ProgramType() = default;
	ProgramType(const ParameterType& inputType, int inputLength, const ParameterType& outputType, int outputLength);

	virtual void read(std::istream& is);
	virtual void write(std::ostream& os) const;

	const ParameterType& getInputType() const;	//入力の型
	int getInputLength() const;					//入力の数
	const ParameterType& getOutputType() const;	//出力の型
	int getOutputLength() const;					//出力の数

	std::vector<double> clipInput(const std::vector<double>& input) const;
	bool acceptsInput(const std::vector<double>& input) const;

	std::vector<double> clipOutput(const std::vector<double>& output) const;
	bool acceptsOutput(const std::vector<double>& output) const;

private:
	ParameterType inputType;
	int inputLength = 0;
	ParameterType outputType;
	int outputLength = 0;
};

#endif /* PROGRAMTYPE_H_ */
