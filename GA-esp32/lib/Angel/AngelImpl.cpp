/*
 * AngelImpl.cpp
 *
 *  Created on: 5 okt. 2021
 *      Author: wilbert
 */

#include <Angel.hpp>
#include <stddef.h>
#include <Time24.hpp>
#include <Time24Factory.hpp>
#include <WatchInterval.hpp>
#include <WatchIntervalFactory.hpp>
#include <algorithm>
#include <iterator>
#include <memory>
#include <string>
#include <vector>
#include <iostream>

class AngelImpl: public Angel {
public:
	AngelImpl(const std::string &phoneNr);

	virtual ~AngelImpl();

	virtual const std::string& getPhoneNr() const;

    virtual void reset();
    virtual void timeProgress(std::shared_ptr<Time24> now);
    virtual void activityDetected();
    virtual bool helpNeeded();

	virtual void addInterval(const std::string &start, const std::string &end);
	virtual void delInterval(const std::string &start, const std::string &end);
	virtual int getNrIntervals() const;
	virtual std::shared_ptr<WatchInterval> getInterval(size_t idx) const;

private:
	std::string phoneNr;
	std::vector<std::shared_ptr<WatchInterval>> intervals;
};

namespace AngelFactory {
std::shared_ptr<Angel> create(const std::string &phoneNr) {
	return std::shared_ptr<Angel>(new AngelImpl(phoneNr));
}
}

AngelImpl::AngelImpl(const std::string &pnr)
: phoneNr(pnr)
{
}

AngelImpl::~AngelImpl() {
}

const std::string& AngelImpl::getPhoneNr() const {
	return phoneNr;
}

void AngelImpl::reset() {
	std::for_each(intervals.begin(), intervals.end(), [](auto i){
		i->reset();
	});
}

void AngelImpl::timeProgress(std::shared_ptr<Time24> now) {
	std::for_each(intervals.begin(), intervals.end(), [&](auto i){
		i->timeProgress(now);
	});
}

void AngelImpl::activityDetected() {
	std::for_each(intervals.begin(), intervals.end(), [&](auto i){
		i->activityDetected();
	});
}

bool AngelImpl::helpNeeded() {
	return std::any_of(intervals.begin(), intervals.end(), [&](auto i){
		return i->getState() == IntervalState::HELPNEEDED;
	});
}

void AngelImpl::addInterval(const std::string &start, const std::string &end) {

	auto startTime = Time24Factory::create(start == "" ? "7:00" : start);
	auto endTime = Time24Factory::create(end == "" ? "10:00" : end);

	if (startTime.get() == nullptr)
		return;
	if (endTime.get() == nullptr)
		return;

	auto newInterval = WatchIntervalFactory::create(startTime, endTime);

	intervals.push_back(newInterval);
}

void AngelImpl::delInterval(const std::string &start, const std::string &end) {
	auto startTime = Time24Factory::create(start);
	auto endTime = Time24Factory::create(end);

	if (startTime.get() == nullptr)
		return;
	if (endTime.get() == nullptr)
		return;

	auto newEnd = std::remove_if(intervals.begin(), intervals.end(),
			[&](std::shared_ptr<WatchInterval> iv) {
				auto result = iv->matches(startTime, endTime);
				return result;
			});
	if (newEnd != intervals.end()) {
		intervals.erase(newEnd, intervals.end());
	}
}

int AngelImpl::getNrIntervals() const {
	return intervals.size();
}

std::shared_ptr<WatchInterval> AngelImpl::getInterval(size_t idx) const {
	if (idx < intervals.size())
		return intervals[idx];
	else
		return nullptr;
}


