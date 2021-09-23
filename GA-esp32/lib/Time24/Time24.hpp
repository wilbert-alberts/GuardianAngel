/*
 * Time24.hpp
 *
 *  Created on: 19 sep. 2021
 *      Author: wilbert
 */

#ifndef TIME24_HPP_
#define TIME24_HPP_

#include <string>

class Time24 {
public:
	virtual ~Time24() {
	}
	;

	virtual int getHours() const = 0;
	virtual int getMinutes() const = 0;
	virtual int getSeconds() const = 0;

	virtual void add(int h, int m, int s) = 0;
	virtual void add(const Time24& other) = 0;
	virtual void addHours(int h) = 0;
	virtual void addMinutes(int m) = 0;
	virtual void addSeconds(int s) = 0;

	virtual int compareTo(const Time24 &other) const = 0;

	virtual std::string toString() const = 0;
};

#endif /* TIME24_HPP_ */
