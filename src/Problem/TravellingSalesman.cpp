/*
 * TravellingSalesman.cpp
 *
 *  Created on: 2013/05/13
 *      Author: kryozahiro
 */

#include <random>
#include <boost/lexical_cast.hpp>
#include "TravellingSalesman.h"
using namespace std;

TravellingSalesman::TravellingSalesman(int size, int seed) :
		Problem(ProgramType(DataType(0, 0, 1, 0), DataType(0, size - 1, 1, size))) {
	//都市のランダム生成
	mt19937_64 engine(seed);
	uniform_real_distribution<> dist(-1.0, 1.0);
	for (int i = 0; i < size; ++i) {
		cities.push_back(pair<double, double>(dist(engine), dist(engine)));
	}
}

double TravellingSalesman::evaluate(Program& program) {
	vector<double> path = program(vector<double>());
	assert(getProgramType().getOutputType().accepts(path));
	vector<pair<double, double>> cityList = cities;

	pair<double, double> prev = cityList[path[0]];
	cityList.erase(cityList.begin() + path[0]);

	//経路の長さを計算する
	double length = 0;
	for (vector<int>::size_type i = 1; i < path.size(); ++i) {
		int cityIndex = ((int)path[i]) % cityList.size();
		pair<double, double> next = cityList[cityIndex];
		cityList.erase(cityList.begin() + cityIndex);

		length += (next.first - prev.first) * (next.first - prev.first) + (next.second - prev.second) * (next.second - prev.second);
		prev = next;
	}
	return length;
}

string TravellingSalesman::toString() const {
	string ret;
	for (vector<pair<int, int>>::size_type i = 0; i < cities.size(); ++i) {
		ret += boost::lexical_cast<string>(cities[i].first) + " " + boost::lexical_cast<string>(cities[i].second) + "\n";
	}
	return ret;
}

/*string TravellingSalesman::showProgram(Program& program) const {
	string ret;

	vector<double> path = program(vector<double>());
	assert(getProgramType().acceptsOutput(path));
	vector<pair<double, double>> cityList = cities;

	pair<double, double> prev = cityList[path[0]];
	cityList.erase(cityList.begin() + path[0]);
	ret += boost::lexical_cast<string>(prev.first) + " " + boost::lexical_cast<string>(prev.second) + "\n";

	//経路の長さを計算する
	for (vector<int>::size_type i = 1; i < path.size(); ++i) {
		int cityIndex = ((int)path[i]) % cityList.size();
		pair<double, double> next = cityList[cityIndex];
		cityList.erase(cityList.begin() + cityIndex);

		ret += boost::lexical_cast<string>(next.first) + " " + boost::lexical_cast<string>(next.second) + "\n";
	}
	ret += boost::lexical_cast<string>(prev.first) + " " + boost::lexical_cast<string>(prev.second) + "\n";
	return ret;
}*/
