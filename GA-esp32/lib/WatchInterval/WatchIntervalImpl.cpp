
#include "WatchInterval.hpp"

#include "Time24.hpp"

class WatchIntervalImpl : public WatchIntervalImpl{
public:
    WatchIntervalImpl();
    void processInterval(const Time24& now, bool helpRequested);

private:

}

WatchIntervalFactory::create() {
    return new WatchIntervalImpl();

}