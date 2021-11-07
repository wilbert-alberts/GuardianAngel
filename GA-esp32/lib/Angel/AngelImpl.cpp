/*
 * AngelImpl.cpp
 *
 *  Created on: 5 okt. 2021
 *      Author: wilbert
 */

#include <memory>
#include <vector>
#include <string>
#include <algorithm>

#include "Angel.hpp"
#include "Time24Interval.hpp"
#include "Time24Factory.hpp"
#include "Time24IntervalFactory.hpp"
#include "WatchDog.hpp"
#include "ActivityDetector.hpp"
#include "GSM.hpp"

#include "AlarmHandler.hpp"

class AngelImpl: public Angel, public ActivityListener, public AlarmHandler {
public:
	AngelImpl(const std::string &phoneNr, std::shared_ptr<WatchDog>,
			std::shared_ptr<ActivityDetector> hr, std::shared_ptr<GSM> gsm);

	virtual ~AngelImpl();

	virtual const std::string& getPhoneNr() const;
	virtual void addInterval(const std::string &start, const std::string &end);
	virtual void delInterval(const std::string &start, const std::string &end);
	virtual int getNrIntervals() const;
	virtual std::shared_ptr<Time24Interval> getInterval(size_t idx) const;
	virtual void activityDetected();
	virtual void raiseAlarm();

private:
	std::string phoneNr;
	std::shared_ptr<WatchDog> wd;
	std::shared_ptr<GSM> gsm;
	std::shared_ptr<ActivityDetector> hr;
	std::vector<std::shared_ptr<Time24Interval>> intervals;
};

namespace AngelFactory {
std::shared_ptr<Angel> create(const std::string &phoneNr,
		std::shared_ptr<WatchDog> wd, std::shared_ptr<ActivityDetector> hr,
		std::shared_ptr<GSM> gsm) {
	return std::shared_ptr<Angel>(new AngelImpl(phoneNr, wd, hr, gsm));
}
}

AngelImpl::AngelImpl(const std::string &pnr, std::shared_ptr<WatchDog> _wd,
		std::shared_ptr<ActivityDetector> _hr, std::shared_ptr<GSM> _gsm) :
		phoneNr(pnr), wd(_wd), gsm(_gsm) {
	hr->addListener(std::shared_ptr<ActivityListener>(this));
}

AngelImpl::~AngelImpl() {
	hr->delListener(std::shared_ptr<ActivityListener>(this));
	std::for_each(intervals.begin(), intervals.end(),
			[&](std::shared_ptr<Time24Interval> i) {
				wd->delInterval(std::shared_ptr<AlarmHandler>(this), i);
			});
}

const std::string& AngelImpl::getPhoneNr() const {
	return phoneNr;
}

void AngelImpl::addInterval(const std::string &start, const std::string &end) {

	auto startTime = Time24Factory::create(start == "" ? "7:00" : start);
	auto endTime = Time24Factory::create(end == "" ? "10:00" : end);

	if (startTime.get() == nullptr)
		return;
	if (endTime.get() == nullptr)
		return;

	auto t24iv = Time24IntervalFactory::create(*startTime, *endTime);

	intervals.push_back(t24iv);

	wd->addInterval(std::shared_ptr<AlarmHandler>(this), t24iv);
}

void AngelImpl::delInterval(const std::string &start, const std::string &end) {
	auto startTime = Time24Factory::create(start);
	auto endTime = Time24Factory::create(end);

	if (startTime.get() == nullptr)
		return;
	if (endTime.get() == nullptr)
		return;

	auto newEnd = std::remove_if(intervals.begin(), intervals.end(),
			[&](std::shared_ptr<Time24Interval> iv) {
				return iv->startsAt(*startTime) && iv->endsAt(*endTime);
			});
	if (newEnd != intervals.end()) {
		intervals.erase(newEnd, intervals.end());
		return;
	}
	newEnd = std::remove_if(intervals.begin(), intervals.end(),
			[&](std::shared_ptr<Time24Interval> iv) {
				return iv->startsAt(*startTime);
			});
	intervals.erase(newEnd, intervals.end());
}

int AngelImpl::getNrIntervals() const {
	return intervals.size();
}

std::shared_ptr<Time24Interval> AngelImpl::getInterval(size_t idx) const {
	if (idx < intervals.size())
		return intervals[idx];
	else
		return nullptr;
}

void AngelImpl::activityDetected() {
	raiseAlarm();
}

void AngelImpl::raiseAlarm() {

}

