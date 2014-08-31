/*
 * Timer.cpp
 *
 *  Created on: 2013/05/15
 *      Author: kryozahiro
 */

#include "Timer.h"
using namespace std;

const std::chrono::high_resolution_clock::time_point Timer::EPOCH = std::chrono::high_resolution_clock::time_point();

int Timer::profile(function<void ()> fn, int times) {
	Timer profiler;
	for (int i = 0; i < times; ++i) {
		fn();
	}
	return profiler.stop();
}

Timer::Timer() {
	totalTime = 0;
	startTime = EPOCH;
}

void Timer::start() {
	totalTime = 0;
	startTime = chrono::high_resolution_clock::now();
}

int Timer::stop() {
	totalTime = elapsed();
	startTime = EPOCH;
	return totalTime;
}

void Timer::resume() {
	startTime = chrono::high_resolution_clock::now();
}

int Timer::elapsed() const {
	int advance = 0;
	if (startTime != EPOCH) {
		auto duration = chrono::high_resolution_clock::now() - startTime;
		advance = chrono::duration_cast<chrono::milliseconds>(duration).count();
	}
	return totalTime + advance;
}
