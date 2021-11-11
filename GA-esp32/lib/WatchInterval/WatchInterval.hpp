
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

	virtual bool insideInterval(const Time24 &now) const = 0;
	virtual bool startsAt(const Time24& t) const = 0;
	virtual bool endsAt(const Time24& t) const = 0;

	virtual const Time24& getStart() const = 0;
	virtual const Time24& getEnd() const = 0;

    virtual bool matches(std::shared_ptr<Time24> start, std::shared_ptr<Time24> end) const = 0;

    virtual void progress(std::shared_ptr<Time24>  now, int nrActivations) = 0;

    virtual IntervalState getState() const = 0;
    virtual void reset() = 0;
};

#endif
