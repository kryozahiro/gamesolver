/*
 * ParameterType.h
 *
 *  Created on: 2013/11/09
 *      Author: kryozahiro
 */

#ifndef PARAMETERTYPE_H_
#define PARAMETERTYPE_H_

#include "../Util/ReadWritable.h"

class ParameterType : public ReadWritable {
public:
	ParameterType() = default;
	ParameterType(double minimum, double maximum, double unit);

	//ReadWritableの実装
	virtual void read(std::istream& is) override;
	virtual void write(std::ostream& os) const override;

	double getMin() const;
	double getMax() const;
	double getUnit() const;

	double clip(double value) const;
	bool accepts(double value) const;

private:
	double minimum = 0.0;
	double maximum = 0.0;
	double unit = 1.0;
};

#endif /* PARAMETERTYPE_H_ */
