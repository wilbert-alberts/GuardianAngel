
#ifndef WATCHINTERVAL_HPP_
#define WATCHINTERVAL_HPP_

class Time24;

class WatchInterval
{
public:
    virtual ~WatchInterval() {}
    virtual void processInterval(const Time24& now, bool helpRequested) = 0;
};

#endif