#ifndef ANGEL_HPP_
#define ANGEL_HPP_

#include <string>
#include <memory>

class WatchInterval;
class Time24;

class Angel
{
public:
    virtual ~Angel() {};

    virtual void reset() = 0;
    virtual const std::string& getPhoneNr() const = 0;
    virtual void timeProgress(std::shared_ptr<Time24> now) = 0;
    virtual void activityDetected() = 0;
    virtual bool helpNeeded() = 0;

    virtual void addInterval(const std::string& start, const std::string& end) = 0;
    virtual void delInterval(const std::string& start, const std::string& end) = 0;
    virtual int getNrIntervals() const = 0;
    virtual std::shared_ptr<WatchInterval> getInterval(size_t idx) const = 0;
};

#endif
