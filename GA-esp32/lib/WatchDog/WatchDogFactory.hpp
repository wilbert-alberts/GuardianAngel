/*
 * WatchDogFactory.hpp
 *
 *  Created on: 21 Sep 2021
 *      Author: AUTHOR
 */

#ifndef WATCHDOGFACTORY_HPP_
#define WATCHDOGFACTORY_HPP_

#include <memory>

class WatchDog;

namespace WatchDogFactory
{
    std::shared_ptr<WatchDog> create();

} // namespace WatchDogFactory

#endif /* WATCHDOGFACTORY_HPP_ */
