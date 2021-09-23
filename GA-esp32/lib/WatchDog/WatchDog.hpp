/*
 * WatchDog.hpp
 *
 *  Created on: 21 Sep 2021
 *      Author: Wilbert
 */

#ifndef WATCHDOG_HPP_
#define WATCHDOG_HPP_

class WatchDog {
public:
    virtual ~WatchDog() {}
    virtual void configure(const Clock& clock, const ActivityDetector& ad);
    virtual void activate();
    
    virtual void addInterval(const WatchInterval& i) = 0;
    virtual void delInterval(const WatchInterval& i) = 0;
};


#endif /* WATCHDOG_HPP_ */
