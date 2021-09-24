
#include "WatchInterval.hpp"

#include "Time24/Time24.hpp"
#include "WatchInterval/WatchInterval.hpp"
#include "WatchInterval/WatchIntervalFactory.hpp"


class WatchIntervalImpl : public WatchInterval {
public:
    WatchIntervalImpl() {}
    virtual ~WatchIntervalImpl() {}
    void processInterval(const Time24& now, bool helpRequested);

private:

};

WatchInterval* WatchIntervalFactory::create() {
    return new WatchIntervalImpl();

}

void WatchIntervalImpl::processInterval(const Time24& now, bool helpRequested) {

}
