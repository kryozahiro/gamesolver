/*
 * MathUtil.h
 *
 *  Created on: 2013/12/02
 *      Author: kryozahiro
 */

#ifndef MATHUTIL_H_
#define MATHUTIL_H_

#include <cmath>
#include <complex>
#include <boost/math/constants/constants.hpp>

namespace MathUtil {

//----------------------------------------
// 定数
//----------------------------------------

//円周率
template <class T>
T pi() {
	static T pi = boost::math::constants::pi<T>();
	return pi;
}

//----------------------------------------
// 角度
//----------------------------------------

//度数からラジアンへ変換
template <class T>
T toRadian(T deg) {
	return deg * pi<T>() / 180.0;
}

//ラジアンから度数へ変換
template <class T>
T toDegree(T rad) {
	return rad * 180.0 / pi<T>();
}

//-PI < rad <= PIに正規化
template <class T>
T normalize(T rad) {
	rad = std::remainder(rad, 2.0 * pi<T>());
	if (rad <= -pi<T>()) {
		rad += 2.0 * pi<T>();
	}
	return rad;
}

//正規化つきの回転
template <class T>
T rotate(T baseRad, T relativeRad) {
	return normalize(baseRad + relativeRad);
}

//----------------------------------------
// 複素数
// テンプレート引数は実数のみ使用可能
//----------------------------------------

//ペアから複素数へ変換
template <class T, class U, class V>
std::complex<T> toComplex(std::pair<U, V> p) {
	return std::complex<T>(p.first, p.second);
}

//複素数からペアへ変換
template <class T, class U, class V>
std::pair<T, U> toPair(std::complex<V> z) {
	return std::make_pair<T, U>(z.real(), z.imag());
}

//単位長への正規化
template <class T>
std::complex<T> normalize(std::complex<T> z) {
	return z / std::abs(z);
}

//----------------------------------------
// ノルムと距離
//----------------------------------------

//実数版Lp-ノルム
template <class T>
T lpNorm(std::complex<T> z, T p) {
	return std::pow(std::pow(std::fabs(z.real()), p) + std::pow(std::fabs(z.imag()), p), 1.0 / p);
}

//実数版Lp-ノルムによる距離
template <class T>
T lpDistance(std::complex<T> z0, std::complex<T> z1, T p) {
	return lpNorm<T>(z1 - z0, p);
}

//整数版Lp-ノルムの実装の詳細
namespace detail {

template <class T, int p>
struct lpNormHelper {
	static T lpNorm(std::complex<T> z) {
		return MathUtil::lpNorm(z, static_cast<T>(p));
	}
};

template <class T>
struct lpNormHelper<T, 1> {
	static T lpNorm(std::complex<T> z) {
		return std::fabs(z.real()) + std::fabs(z.imag());
	}
};

template <class T>
struct lpNormHelper<T, 2> {
	static T lpNorm(std::complex<T> z) {
		return std::abs(z);
	}
};

template <class T>
struct lpNormHelper<T, INT_MAX> {
	static T lpNorm(std::complex<T> z) {
		return std::max(std::fabs(z.real()), std::fabs(z.imag()));
	}
};

}

//整数版Lp-ノルム
template <class T, int p>
T lpNorm(std::complex<T> z) {
	return detail::lpNormHelper<T, p>::lpNorm(z);
}

//整数版Lp-ノルムによる距離
template <class T, int p>
T lpDistance(std::complex<T> z0, std::complex<T> z1) {
	return lpNorm<T, p>(z1 - z0);
}

//マンハッタンノルム
template <class T>
T manhattanNorm(std::complex<T> z) {
	return lpNorm<T, 1>(z);
}

//マンハッタン距離
template <class T>
T manhattanDistance(std::complex<T> z0, std::complex<T> z1) {
	return manhattanNorm(z1 - z0);
}

//ユークリッドノルム
template <class T>
T euclideanNorm(std::complex<T> z) {
	return lpNorm<T, 2>(z);
}

//ユークリッド距離
template <class T>
T euclideanDistance(std::complex<T> z0, std::complex<T> z1) {
	return euclideanNorm(z1 - z0);
}

//チェス盤ノルム
template <class T>
T chessboardNorm(std::complex<T> z) {
	return lpNorm<T, INT_MAX>(z);
}

//チェス盤距離
template <class T>
T chessboardDistance(std::complex<T> z0, std::complex<T> z1) {
	return chessboardNorm(z1 - z0);
}

//----------------------------------------
// アフィン変換
//----------------------------------------

//平行移動
template <class T>
std::complex<T> translate(std::complex<T> z, std::complex<T> translation) {
	return z + translation;
}

//回転
template <class T>
std::complex<T> rotate(std::complex<T> z, double rad) {
	return z * std::polar(1.0, rad);
}

//拡縮
template <class T>
std::complex<T> scale(std::complex<T> z, std::complex<T> scaling) {
	return std::complex<T>(z.real() * scaling.real(), z.imag() * scaling.imag());
}

//変換
template <class T>
std::complex<T> transform(std::complex<T> z, std::complex<T> translation, double rotateRad, std::complex<T> scaling = std::complex<T>(1, 1)) {
	return translate(rotate(scale(z, scaling), rotateRad), translation);
}

}

#endif /* MATHUTIL_H_ */
