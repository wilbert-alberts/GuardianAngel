/*
 * Time24Impl.cpp
 *
 *  Created on: 19 sep. 2021
 *      Author: wilbert
 */
#ifndef TIME24IMPL_HPP_
#define TIME24IMPL_HPP_

#include <Time24.hpp>
#include <string>

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

#endif
