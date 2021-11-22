/*
 * Time24Interval.hpp
 *
 *  Created on: 19 sep. 2021
 *      Author: wilbert
 */

#ifndef DEBOUNCER_HPP_
#define DEBOUNCER_HPP_


#include <ValueProvider.hpp>
#include <functional>

class Debouncer {
public:
	Debouncer(int stabilizationPeriod, ValueProvider valueProvider);

	int getValue() const;
	bool tick();

private:
	std::function<int()> valueProvider;
	int stabilizationPeriod;

	int stableValue;
	int stablePeriods;
	int unstableValue;

};

#endif
