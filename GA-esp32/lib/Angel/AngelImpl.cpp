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

class AngelImpl: public Angel {
public:
	AngelImpl(const std::string &phoneNr);

	virtual ~AngelImpl();

	virtual const std::string& getPhoneNr() const;

    virtual void reset();
    virtual void progress(std::shared_ptr<Time24> now, int nrActiviations);
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

void AngelImpl::progress(std::shared_ptr<Time24> now, int nrActivations) {
	std::for_each(intervals.begin(), intervals.end(), [&](auto i){
		i->progress(now, nrActivations);
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

	auto newInterval = WatchIntervalFactory::create(*startTime, *endTime);

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
				return iv->matches(startTime, endTime);
			});
	if (newEnd != intervals.end()) {
		intervals.erase(newEnd, intervals.end());
		return;
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


