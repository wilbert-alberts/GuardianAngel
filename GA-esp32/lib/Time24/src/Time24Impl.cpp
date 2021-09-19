/*
 * Time24Impl.cpp
 *
 *  Created on: 19 sep. 2021
 *      Author: wilbert
 */

#include <stdexcept>

#include "Time24.hpp"

class Time24Impl: public Time24 {
public:
	Time24Impl(int h, int m, int s = 0);
	virtual ~Time24Impl();

	virtual int getHour() const;
	virtual int getMinute() const;
	virtual int getSecond() const;

	virtual int compareTo(const Time24 &other) const;

private:
	int hour;
	int minute;
	int second;

	void verifyIntegrity();
	void verifyNaturalMax(int value, int bound);
};

namespace Time24Factory {
extern Time24* create(int h, int m, int s=0) {
	return new Time24Impl(h, m, s);
}
}

Time24Impl::Time24Impl(int h, int m, int s) {
	hour = h;
	minute = m;
	second = s;
	verifyIntegrity();
}

Time24Impl::~Time24Impl() {
}

int Time24Impl::getHour() const {
	return hour;
}

int Time24Impl::getMinute() const {
	return minute;
}

int Time24Impl::getSecond() const {
	return second;
}

int Time24Impl::compareTo(const Time24 &other) const {
	if (hour < other.getHour())
		return -1;
	if (hour > other.getHour())
		return 1;
	// hours are equal
	if (minute < other.getMinute())
		return -1;
	if (minute > other.getMinute())
		return 1;

	// hour and minutes are equal
	if (second < other.getSecond())
		return -1;
	if (second > other.getSecond())
		return 1;

	// hour and minutes and seconds are equal
	return 0;
}

void Time24Impl::verifyIntegrity() {
	verifyNaturalMax(hour, 24);
	verifyNaturalMax(minute, 60);
	verifyNaturalMax(second, 60);
}

void Time24Impl::verifyNaturalMax(int value, int bound) {
	if (value < 0)
		throw new std::range_error("Too low");
	if (value >= bound)
		throw new std::range_error("Too high");
}
