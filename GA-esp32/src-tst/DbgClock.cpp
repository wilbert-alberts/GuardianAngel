/*
 * ClockDbg.cpp
 *
 *  Created on: 17 nov. 2021
 *      Author: wilbert
 */

#include "DbgClock.hpp"

#include <Time24.hpp>
#include <Time24Factory.hpp>
#include <algorithm>
#include <iterator>


void DbgClock::setTimes(std::vector<std::string> _times) {

	std::for_each(_times.begin(), _times.end(), [this](auto s) {
		times.push_back(Time24Factory::create(s));
	});
}

void DbgClock::pushTime(std::string time) {
	times.push_back(Time24Factory::create(time));
}


std::shared_ptr<Time24> DbgClock::getTime() const {
	if (times.size() > 0) {
		return *times.begin();
	} else {
		return std::shared_ptr<Time24>(nullptr);
	}
}

void DbgClock::advanceTime() {
	if (times.size() > 0) {
		times.erase(times.begin());
	}
}

void DbgClock::setGSM(std::shared_ptr<GSM> gsm) {

}
