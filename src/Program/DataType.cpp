/*
 * DataType.cpp
 *
 *  Created on: 2015/01/25
 *      Author: kryozahiro
 */

#include <tuple>
#include "CppUtil/GenericIo.h"
#include "DataType.h"
using namespace std;

DataType::DataType(double minimum, double maximum, double unit, int size)
	: minimum(minimum), maximum(maximum), unit(unit), size(size) {
}

void DataType::read(istream& is) {
	tuple<double, double, double, int> data;
	is >> data;
	minimum = get<0>(data);
	maximum = get<1>(data);
	unit = get<2>(data);
	size = get<3>(data);
}

void DataType::write(ostream& os) const {
	tuple<double, double, double, int> data(minimum, maximum, unit, size);
	os << data;
}

double DataType::getMin() const {
	return minimum;
}

double DataType::getMax() const {
	return maximum;
}

double DataType::getUnit() const {
	return unit;
}

int DataType::getSize() const {
	return size;
}

bool DataType::accepts(const std::vector<double>& data) const {
	vector<double> tester(data);
	clip(tester);
	return data == tester;
}

void DataType::clip(std::vector<double>& data) const {
	for (double& d : data) {
		d = min(max(minimum, d), maximum);
	}
}

void DataType::scaleFrom(std::vector<double>& data, std::pair<double, double> range) const {
	double ratio = (range.second - range.first) / (maximum - minimum);
	for (double& d : data) {
		d = (d - range.first) * ratio + minimum;
	}
}

void DataType::scaleTo(std::vector<double>& data, std::pair<double, double> range) const {
	double ratio = (maximum - minimum) / (range.second - range.first);
	for (double& d : data) {
		d = (d - minimum) * ratio + range.first;
	}
}
