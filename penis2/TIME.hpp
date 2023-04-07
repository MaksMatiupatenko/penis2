#ifndef __TIMEHPP__
#define __TIMEHPP__

#include "BASE.h"
#include <chrono>

typedef decltype(std::chrono::high_resolution_clock::now()) TIME_T;

TIME_T getTime() {
	return (std::chrono::high_resolution_clock::now());
}

TIME_T setTime(TIME_T& prevTime) {
	return prevTime = getTime();
}

FLOAT64 getTimeDiff(const TIME_T& time) {
	return std::chrono::duration_cast<std::chrono::milliseconds>(getTime() - time).count() / 1e3;
}

FLOAT64 getCurTime() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(getTime().time_since_epoch()).count() / 1e3;
}

#endif