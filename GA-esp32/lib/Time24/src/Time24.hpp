/*
 * Time24.hpp
 *
 *  Created on: 19 sep. 2021
 *      Author: wilbert
 */

#ifndef TIME24_HPP_
#define TIME24_HPP_

class Time24 {
public:
	virtual ~Time24() {
	}
	;

	virtual int getHour() const = 0;
	virtual int getMinute() const = 0;
	virtual int getSecond() const = 0;

	virtual int compareTo(const Time24 &other) const = 0;
};

#endif /* TIME24_HPP_ */
