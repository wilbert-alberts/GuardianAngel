/*
 * ClockDbg.cpp
 *
 *  Created on: 17 nov. 2021
 *      Author: wilbert
 */

#include <Clock.hpp>
#include <Time24Factory.hpp>
#include <algorithm>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

class TimeTable {
public:
	void setTimes(std::vector<std::string> times);
	std::shared_ptr<Time24> popTime();

private:
	std::vector<std::shared_ptr<Time24>> times;
};

void TimeTable::setTimes(std::vector<std::string> _times) {
	std::transform(_times.begin(), _times.end(), times.begin(), [](auto s) {
		return Time24Factory::create(s);
	});
}
std::shared_ptr<Time24> TimeTable::popTime() {
	if (times.size() > 0) {
		auto r = *times.begin();
		times.erase(times.begin());
		return r;
	} else {
		return std::shared_ptr<Time24>(nullptr);
	}
}

class DbgClock: public Clock {
public:
	virtual ~DbgClock() {
	}
	;
	void setTimes(std::vector<std::string> times);
	virtual std::shared_ptr<Time24> getTime() const;
	virtual void setGSM(std::shared_ptr<GSM> gsm) = 0;
private:
	TimeTable tt;
};

void DbgClock::setTimes(std::vector<std::string> times) {
	tt.setTimes(times);
}

std::shared_ptr<Time24> DbgClock::getTime() const {
	return ((TimeTable)tt).popTime();
}

