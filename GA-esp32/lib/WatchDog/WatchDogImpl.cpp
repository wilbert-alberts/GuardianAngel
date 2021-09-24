/*
 * WatchDog.cpp
 *
 *  Created on: 21 Sep 2021
 *      Author: Wilbert
 */

#include "WatchDog.hpp"
#include "WatchDogFactory.hpp"

#include "WatchInterval/WatchInterval.hpp"
#include "Clock/Clock.hpp"

#include <set>
#include <algorithm>


class WatchDogImpl : public WatchDog
{
public:
    WatchDogImpl() {};
    virtual ~WatchDogImpl() {}
    virtual void addInterval(const WatchInterval &i);
    virtual void delInterval(const WatchInterval &i);

private:
    void processIntervals();

    const Clock * clock;
//    const ActivityDetector&  activityDetector;

//    std::set<const WatchInterval &> intervals;
};

WatchDog* WatchDogFactory::create() {
    return new WatchDogImpl();
}



void WatchDogImpl::addInterval(const WatchInterval &i)
{
//    intervals.insert(i);
}

void WatchDogImpl::delInterval(const  WatchInterval &i)
{
//    intervals.erase(i);
}

void WatchDogImpl::processIntervals() {
//    auto now = clock->getTime();
//    bool activity = activityDetector.isActivityDetected();

//    for (auto iter = intervals.begin(); iter != intervals.end(); iter++) {
//        iter->processInterval(now, activity);
//    }
} 


