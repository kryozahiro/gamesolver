/*
 * Scale.h
 *
 *  Created on: 2014/01/05
 *      Author: kryozahiro
 */

#ifndef SCALE_H_
#define SCALE_H_

namespace Scale {

//線形スケーリング

//y = ax + b
template <class T>
std::function<T(T)> linerScaler(T a, T b) {
	return [=](T t) -> T {
		return t * a + b;
	};
}

//範囲の線形変換
template <class T>
std::function<T(T)> linearScaler(std::pair<T, T> fromMinMax, std::pair<T, T> toMinMax) {
	T fromSize = fromMinMax.second - fromMinMax.first;
	T toSize = toMinMax.second - toMinMax.first;
	T a = toSize / fromSize;
	T b = toMinMax.first - fromMinMax.first * a;
	return linerScaler(a, b);
}

//全体の平均を保って最大値を平均のscale倍にするような変換
//f(mean) = a * mean + b = average
//f(max) = a * max + b = scale * average
template <class T>
std::function<T(T)> linearScaler(T mean, T max, T scale) {
	T a = mean * (scale - 1.0) / (max - mean);
	T b = mean * (1.0 - a);
	return linerScaler(a, b);
}

//シグマスケーリング
//sigma truncation
//y = x − (mean - scale * sd)
//平均と標準偏差
template <class T>
std::function<T(T)> simgaScaler(T mean, T sd, T scale) {
	return [=](T t) -> T {
		return t - mean + scale * sd;
	};
}

}

#endif /* SCALE_H_ */
