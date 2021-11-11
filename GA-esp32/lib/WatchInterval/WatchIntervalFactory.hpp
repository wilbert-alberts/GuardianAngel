#ifndef WATCHINTERVALFACTORY_HPP_
#define WATCHINTERVALFACTORY_HPP_

#include <memory>


class AlarmHandler;
class Time24Interval;
class WatchInterval;

namespace WatchIntervalFactory {

std::shared_ptr<WatchInterval> create(const Time24 &start, const Time24 &end);

}

#endif
