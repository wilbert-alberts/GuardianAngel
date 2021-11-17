#ifndef WATCHINTERVALFACTORY_HPP_
#define WATCHINTERVALFACTORY_HPP_

#include <memory>


class AlarmHandler;
class Time24Interval;
class WatchInterval;

namespace WatchIntervalFactory {

std::shared_ptr<WatchInterval> create(std::shared_ptr<Time24> start, std::shared_ptr<Time24> end);

}

#endif
