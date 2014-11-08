/*
 * ParameterType.cpp
 *
 *  Created on: 2013/11/09
 *      Author: kryozahiro
 */

#include <algorithm>
#include "CppUtil/GenericIo.h"
#include "ParameterType.h"
using namespace std;

ParameterType::ParameterType(double minimum, double maximum, double unit)
	: minimum(minimum), maximum(maximum), unit(unit) {
}

void ParameterType::read(istream& is) {
	vector<double> data;
	is >> data;
	minimum = data[0];
	maximum = data[1];
	unit = data[2];
}

void ParameterType::write(ostream& os) const {
	vector<double> data = {minimum, maximum, unit};
	os << data;
}

double ParameterType::getMin() const {
	return minimum;
}

double ParameterType::getMax() const {
	return maximum;
}

double ParameterType::getUnit() const {
	return unit;
}

double ParameterType::clip(double value) const {
	return min(max(minimum, value), maximum);
}

bool ParameterType::accepts(double value) const {
	return value == clip(value);
}
