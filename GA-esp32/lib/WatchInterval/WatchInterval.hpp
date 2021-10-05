
#ifndef WATCHINTERVAL_HPP_
#define WATCHINTERVAL_HPP_

#include <memory>

class Time24;
class Time24Interval;

class WatchInterval
{
public:
    virtual ~WatchInterval() {}
    virtual bool matches(std::shared_ptr<Time24Interval> other) = 0;
    virtual void advanceTime(std::shared_ptr<Time24>  now) = 0;
    virtual void activityDetected(std::shared_ptr<Time24>  now) = 0;
};

#endif
