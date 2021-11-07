#ifndef ANGEL_HPP_
#define ANGEL_HPP_

#include <string>
#include <memory>

class Time24Interval;

class Angel
{
public:
    virtual ~Angel() {};

    virtual const std::string& getPhoneNr() const = 0;
    virtual void addInterval(const std::string& start, const std::string& end) = 0;
    virtual void delInterval(const std::string& start, const std::string& end) = 0;
    virtual int getNrIntervals() const = 0;
    virtual std::shared_ptr<Time24Interval> getInterval(size_t idx) const = 0;
};

#endif
