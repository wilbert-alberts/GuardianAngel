/*
 * AngelImpl.cpp
 *
 *  Created on: 5 okt. 2021
 *      Author: wilbert
 */

#include "platform.hpp"

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

class AngelImpl : public Angel
{
public:
	AngelImpl(const std::string &phoneNr);

	virtual ~AngelImpl();

	virtual const std::string &getPhoneNr() const;

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

namespace AngelFactory
{
	std::shared_ptr<Angel> create(const std::string &phoneNr)
	{
		return std::shared_ptr<Angel>(new AngelImpl(phoneNr));
	}
}

AngelImpl::AngelImpl(const std::string &pnr)
	: phoneNr(pnr)
{
	LOG_ENTRY("angel phoneNr: %s", pnr.c_str());
	LOG_EXIT();
}

AngelImpl::~AngelImpl()
{
	LOG_ENTRY("angel phoneNr: %s", phoneNr.c_str());
	LOG_EXIT();
}

const std::string &AngelImpl::getPhoneNr() const
{
	LOG_ENTRY();
	LOG_EXIT("angel phoneNr: %s", phoneNr.c_str());
	return phoneNr;
}

void AngelImpl::reset()
{
	LOG_ENTRY();
	//	std::for_each(intervals.begin(), intervals.end(), [](auto i){
	std::for_each(intervals.begin(), intervals.end(), [](std::shared_ptr<WatchInterval> i)
				  { i->reset(); });
	LOG_EXIT();
}

void AngelImpl::timeProgress(std::shared_ptr<Time24> now)
{
	LOG_ENTRY("now: %s", now->toString().c_str());
	std::for_each(intervals.begin(), intervals.end(), [&](std::shared_ptr<WatchInterval> i)
				  { i->timeProgress(now); });
	LOG_EXIT();
}

void AngelImpl::activityDetected()
{
	LOG_ENTRY();
	std::for_each(intervals.begin(), intervals.end(), [](std::shared_ptr<WatchInterval> i)
				  { i->activityDetected(); });
	LOG_EXIT();
}

bool AngelImpl::helpNeeded()
{
	LOG_ENTRY();
	bool result = std::any_of(intervals.begin(), intervals.end(), [&](std::shared_ptr<WatchInterval> i)
							  { return i->getState() == IntervalState::HELPNEEDED; });
	LOG_EXIT("result: %s", boolToString(result));
	return result;
}

void AngelImpl::addInterval(const std::string &start, const std::string &end)
{
	LOG_ENTRY("start: %s, end: %s", start.c_str(), end.c_str());

	auto startTime = Time24Factory::create(start == "" ? "7:00" : start);
	auto endTime = Time24Factory::create(end == "" ? "10:00" : end);

	if (startTime.get() != nullptr && endTime.get() != nullptr)
	{

		auto newInterval = WatchIntervalFactory::create(startTime, endTime);
		intervals.push_back(newInterval);
	}
	LOG_EXIT("nrIntervals: %d", intervals.size());
}

void AngelImpl::delInterval(const std::string &start, const std::string &end)
{
	LOG_ENTRY("start: %s, end: %s", start.c_str(), end.c_str());

	auto startTime = Time24Factory::create(start);
	auto endTime = Time24Factory::create(end);

	if (startTime.get() != nullptr && endTime.get() != nullptr)
	{
		auto newEnd = std::remove_if(intervals.begin(), intervals.end(),
									 [&](std::shared_ptr<WatchInterval> iv)
									 {
										 auto result = iv->matches(startTime, endTime);
										 return result;
									 });
		if (newEnd != intervals.end())
		{
			intervals.erase(newEnd, intervals.end());
		}
	}
	LOG_EXIT("nrIntervals: %d", intervals.size());
}

int AngelImpl::getNrIntervals() const
{
	LOG_ENTRY();
	LOG_EXIT("nrIntervals: %d", intervals.size());
	return intervals.size();
}

std::shared_ptr<WatchInterval> AngelImpl::getInterval(size_t idx) const
{
	LOG_ENTRY();
	std::shared_ptr<WatchInterval> result = nullptr;
	if (idx < intervals.size())
		result = intervals[idx];

	if (result != nullptr)
		LOG_EXIT("result.start: %s, result.end: %s", result->getStart()->toString().c_str(), result->getEnd()->toString().c_str());
	else
		LOG_EXIT("result: nullptr");
	return result;
}
