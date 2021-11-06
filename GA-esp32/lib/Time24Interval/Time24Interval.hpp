/*
 * Time24Interval.hpp
 *
 *  Created on: 19 sep. 2021
 *      Author: wilbert
 */

#ifndef TIME24INTERVAL_HPP_
#define TIME24INTERVAL_HPP_

#include "Time24.hpp"

#include <memory>

class Time24Interval {
public:
	virtual ~Time24Interval() {
	}
	;

	virtual bool insideInterval(const Time24 &now) const = 0;
	virtual bool startsAt(const Time24& t) const = 0;
	virtual bool endsAt(const Time24& t) const = 0;

	virtual const Time24& getStart() const = 0;
	virtual const Time24& getEnd() const = 0;
};

#endif /* TIME24INTERVAL_HPP_ */
