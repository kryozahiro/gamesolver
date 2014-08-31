/*
 * Timer.h
 *
 *  Created on: 2013/05/15
 *      Author: kryozahiro
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <functional>
#include <chrono>

class Timer {
public:
	//fnをtimes回実行して経過したミリ秒を返す
	static int profile(std::function<void ()> fn, int times = 1);

	Timer();
	void start();
	int stop();
	void resume();
	int elapsed() const;

private:
	static const std::chrono::high_resolution_clock::time_point EPOCH;
	std::chrono::milliseconds::rep totalTime;
	std::chrono::high_resolution_clock::time_point startTime;
};

#endif /* TIMER_H_ */
