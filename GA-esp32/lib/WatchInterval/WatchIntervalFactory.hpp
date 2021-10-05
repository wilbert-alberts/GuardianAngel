#ifndef WATCHINTERVALFACTORY_HPP_
#define WATCHINTERVALFACTORY_HPP_

#include <memory>


class Angel;
class Time24Interval;
class WatchInterval;

namespace WatchIntervalFactory {

std::shared_ptr<WatchInterval> create(std::shared_ptr<Angel> angel, std::shared_ptr<Time24Interval> iv);

}

#endif
