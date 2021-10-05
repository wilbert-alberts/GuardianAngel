/*
 * Time24Interval.hpp
 *
 *  Created on: 19 sep. 2021
 *      Author: wilbert
 */

#ifndef TIME24INTERVAL_HPP_
#define TIME24INTERVAL_HPP_

class Time24;

class Time24Interval {
public:
	virtual ~Time24Interval() {
	}
	;

	virtual bool insideInterval(const Time24 &now) = 0;
	virtual bool startsAt(const Time24& t) = 0;
	virtual bool endsAt(const Time24& t) = 0;
};

#endif /* TIME24INTERVAL_HPP_ */
