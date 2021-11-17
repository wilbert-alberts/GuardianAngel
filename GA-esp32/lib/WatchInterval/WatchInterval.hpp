
#ifndef WATCHINTERVAL_HPP_
#define WATCHINTERVAL_HPP_

#include <memory>

class Time24;
class Time24Interval;
class AlarmHandler;

typedef enum {
	PASSIVE,
	ACTIVE,
	HELPNEEDED
} IntervalState;

class WatchInterval
{
public:
    virtual ~WatchInterval() {}

	virtual bool insideInterval(std::shared_ptr<Time24> now) const = 0;
	virtual bool startsAt(std::shared_ptr<Time24> now) const = 0;
	virtual bool endsAt(std::shared_ptr<Time24> now) const = 0;

	virtual std::shared_ptr<const Time24> getStart() const = 0;
	virtual std::shared_ptr<const Time24> getEnd() const = 0;

    virtual bool matches(std::shared_ptr<Time24> start, std::shared_ptr<Time24> end) const = 0;

    virtual void activityDetected(int nrActivations=1) = 0;
    virtual void timeProgress(std::shared_ptr<Time24> now) = 0;

    virtual IntervalState getState() const = 0;
    virtual void reset() = 0;
};

#endif
