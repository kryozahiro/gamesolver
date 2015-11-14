/*
 * ProgramType.h
 *
 *  Created on: 2013/11/09
 *      Author: kryozahiro
 */

#ifndef PROGRAMTYPE_H_
#define PROGRAMTYPE_H_

#include "DataType.h"

//プログラムの入出力型
class ProgramType : public cpputil::ReadWritable {
public:
	ProgramType() = default;
	ProgramType(const DataType& inputType, const DataType& outputType);
	virtual ~ProgramType() = default;

	//ReadWritableの実装
	virtual void read(std::istream& is) override;
	virtual void write(std::ostream& os) const override;

	//入出力型の取得
	const DataType& getInputType() const;
	const DataType& getOutputType() const;

private:
	DataType inputType;
	DataType outputType;
};

#endif /* PROGRAMTYPE_H_ */
