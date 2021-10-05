/*
 * WatchDog.cpp
 *
 *  Created on: 21 Sep 2021
 *      Author: Wilbert
 */

#include <memory>
#include <vector>
#include <algorithm>

#include "WatchDog.hpp"
#include "WatchDogFactory.hpp"

#include "PeriodicTask.hpp"

#include "WatchInterval.hpp"
#include "WatchIntervalFactory.hpp"
#include "Clock.hpp"
#include "ActivityDetector.hpp"

class WatchDogImpl: public WatchDog,
		public PeriodicTask,
		public ActivityListener {
public:
	WatchDogImpl();
	virtual ~WatchDogImpl() {
	}

	virtual void setClock(std::shared_ptr<Clock> cl);
	virtual void addInterval(std::shared_ptr<Angel> a,
			std::shared_ptr<Time24Interval> i);
	virtual void delInterval(std::shared_ptr<Angel> a,
			std::shared_ptr<Time24Interval> i);

	// ActivityListener
	virtual void setActivityDetector(std::shared_ptr<ActivityDetector> p);
	virtual void activityDetected();

	// PeriodicTask
	virtual void tick();

private:

	std::shared_ptr<Clock> clock;
	std::vector<std::shared_ptr<WatchInterval>> intervals;
};

std::shared_ptr<WatchDog> WatchDogFactory::create() {
	return std::shared_ptr<WatchDog>(new WatchDogImpl());
}

WatchDogImpl::WatchDogImpl() :
		PeriodicTask("WatchDoc", 60000, 4000), clock(nullptr) {
}

void WatchDogImpl::setClock(std::shared_ptr<Clock> cl) {
	clock = cl;
}

void WatchDogImpl::setActivityDetector(std::shared_ptr<ActivityDetector> ad) {
	ad->addListener(std::shared_ptr<ActivityListener>(this));
}

void WatchDogImpl::activityDetected() {
	if (clock != nullptr) {
		auto now = clock->getTime();
		std::for_each(intervals.begin(), intervals.end(),
				[&](std::shared_ptr<WatchInterval> i) {
					i->activityDetected(now);
				});
	}
}

void WatchDogImpl::addInterval(std::shared_ptr<Angel> a,
		std::shared_ptr<Time24Interval> i) {
	std::shared_ptr<WatchInterval> wi = WatchIntervalFactory::create(a, i);
	intervals.emplace_back(wi);
}

void WatchDogImpl::delInterval(std::shared_ptr<Angel> a,
		std::shared_ptr<Time24Interval> i) {
	auto newEnd = std::remove_if(intervals.begin(), intervals.end(),
			[&](std::shared_ptr<WatchInterval> e) {
				return e->matches(i);
			});
	intervals.erase(newEnd, intervals.end());
}

void WatchDogImpl::tick() {
	if (clock != nullptr) {
		auto now = clock->getTime();
		std::for_each(intervals.begin(), intervals.end(),
				[&](std::shared_ptr<WatchInterval> i) {
					i->advanceTime(now);
				});
	}
}

