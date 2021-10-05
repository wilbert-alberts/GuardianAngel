/*
 * WatchDog.hpp
 *
 *  Created on: 21 Sep 2021
 *      Author: Wilbert
 */

#ifndef WATCHDOG_HPP_
#define WATCHDOG_HPP_

#include <memory>

class Time24Interval;
class Clock;
class ActivityDetector;
class Angel;

class WatchDog {
public:
    virtual ~WatchDog() {}
    virtual void setClock(std::shared_ptr<Clock> cl) = 0;
    virtual void setActivityDetector(std::shared_ptr<ActivityDetector> p) = 0;
    virtual void addInterval(std::shared_ptr<Angel> angel, std::shared_ptr<Time24Interval> i) = 0;
    virtual void delInterval(std::shared_ptr<Angel> angel, std::shared_ptr<Time24Interval> i) = 0;
};


#endif /* WATCHDOG_HPP_ */
