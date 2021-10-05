/*
 * Time24Impl.cpp
 *
 *  Created on: 19 sep. 2021
 *      Author: wilbert
 */

#include <stdexcept>
#include <memory>
#include <string>
#include <sstream>

#include "Time24.hpp"

class Time24Impl: public Time24 {
public:
	Time24Impl(int h, int m, int s);
	virtual ~Time24Impl();

	virtual int getHours() const;
	virtual int getMinutes() const;
	virtual int getSeconds() const;

	virtual void add(int h, int m, int s);
	virtual void add(const Time24 &other);
	virtual void addHours(int h);
	virtual void addMinutes(int m);
	virtual void addSeconds(int s);

	virtual int compareTo(const Time24 &other) const;

	virtual std::string toString() const;

private:
	int hours;
	int minutes;
	int seconds;

	void verifyIntegrity();
	void verifyIntegrity(int h, int m, int s);
	void verifyNatural(int value);
	void verifyNaturalMax(int value, int bound);
};

namespace Time24Factory {
std::shared_ptr<Time24> create(int h, int m, int s) {
	return std::shared_ptr<Time24>(new Time24Impl(h, m, s));
}

std::shared_ptr<Time24> create(std::string hhmm) {
	std::string hoursStr = hhmm.substr(0, hhmm.find(":"));
	std::string minsStr = hhmm.substr(hhmm.find(":") + 1);
	try {
		int hours = stoi(hoursStr);
		int mins = stoi(minsStr);

		if (hours >= 0 && hours < 24 && mins >= 0 && mins < 60)
			return create(hours, mins, 0);
		else
			return std::shared_ptr<Time24>(nullptr);
	} catch (...) {
		return std::shared_ptr<Time24>(nullptr);
	}
}
}

Time24Impl::Time24Impl(int h, int m, int s) {
	hours = h;
	minutes = m;
	seconds = s;
	verifyIntegrity();
}

Time24Impl::~Time24Impl() {
}

int Time24Impl::getHours() const {
	return hours;
}

int Time24Impl::getMinutes() const {
	return minutes;
}

int Time24Impl::getSeconds() const {
	return seconds;
}

void Time24Impl::add(int h, int m, int s) {
	addHours(h);
	addMinutes(m);
	addSeconds(s);
}

void Time24Impl::add(const Time24 &other) {
	add(other.getHours(), other.getMinutes(), other.getSeconds());
}

void Time24Impl::addHours(int h) {
	hours = (hours + h) % 24;
}

void Time24Impl::addMinutes(int m) {
	addHours((m + minutes) / 60);
	minutes = (m + minutes) % 60;
}

void Time24Impl::addSeconds(int s) {
	addMinutes((s + seconds) / 60);
	seconds = (s + seconds) % 60;
}

int Time24Impl::compareTo(const Time24 &other) const {
	if (hours < other.getHours())
		return -1;
	if (hours > other.getHours())
		return 1;
	// hours are equal
	if (minutes < other.getMinutes())
		return -1;
	if (minutes > other.getMinutes())
		return 1;

	// hour and minutes are equal
	if (seconds < other.getSeconds())
		return -1;
	if (seconds > other.getSeconds())
		return 1;

	// hour and minutes and seconds are equal
	return 0;
}

void Time24Impl::verifyIntegrity() {
	verifyIntegrity(hours, minutes, seconds);
}

void Time24Impl::verifyIntegrity(int h, int m, int s) {
	verifyNaturalMax(h, 24);
	verifyNaturalMax(m, 60);
	verifyNaturalMax(s, 60);
}

void Time24Impl::verifyNaturalMax(int value, int bound) {
	verifyNatural(value);
	if (value >= bound)
		throw new std::range_error("Too high");
}

void Time24Impl::verifyNatural(int value) {
	if (value < 0)
		throw new std::range_error("Too low");
}

std::string Time24Impl::toString() const {
	std::stringstream ss;
	if (hours < 10)
		ss << ' ';
	ss << hours << ':';
	if (minutes < 10)
		ss << '0';
	ss << minutes << ':';
	if (seconds < 10)
		ss << '0';
	ss << seconds;
	return ss.str();
}
