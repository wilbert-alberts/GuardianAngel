/*
 * Time24Impl.cpp
 *
 *  Created on: 19 sep. 2021
 *      Author: wilbert
 */

#include <Time24Impl.hpp>
#include <sstream>
#include <stdexcept>

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
