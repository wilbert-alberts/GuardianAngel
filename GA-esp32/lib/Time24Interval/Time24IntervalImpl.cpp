/*
 * Time24IntervalImpl.cpp
 *
 *  Created on: 19 sep. 2021
 *      Author: wilbert
 */

#include "Time24/Time24.hpp"
#include "Time24Interval.hpp"
#include "Time24IntervalFactory.hpp"

class Time24IntervalImpl: public Time24Interval {
public:
	Time24IntervalImpl(const Time24 &start, const Time24 &end);
	virtual ~Time24IntervalImpl() {
	}

	virtual bool insideInterval(const Time24 &now);

private:
	const Time24 &start;
	const Time24 &end;

	bool insideStartAndEnd(const Time24 &now);
	bool insideEndAndStart(const Time24 &now);
};

Time24Interval* Time24IntervalFactory::create(const Time24& start, const Time24& end) {
	return new Time24IntervalImpl(start, end);
}

Time24IntervalImpl::Time24IntervalImpl(const Time24 &start, const Time24 &end) :
		start(start), end(end) {
}

bool Time24IntervalImpl::insideInterval(const Time24 &now) {
	if (start.compareTo(end) < 0)
		return insideStartAndEnd(now);
	else
		return insideEndAndStart(now);
}

bool Time24IntervalImpl::insideStartAndEnd(const Time24 &now) {
	if ((now.compareTo(start) >= 0) || (end.compareTo(now) <= 0))
		return true;
	else
		return false;
}
bool Time24IntervalImpl::insideEndAndStart(const Time24 &now) {
	if ((now.compareTo(start) >= 0) || (now.compareTo(end) <= 0))
		return true;
	else
		return false;
}
