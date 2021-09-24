/*
 * ClockFactory.hpp
 *
 *  Created on: 14 sep. 2021
 *      Author: wilbert
 */


#ifndef CLOCKFACTORY_HPP_
#define CLOCKFACTORY_HPP_

#include "Clock.hpp"

namespace ClockFactory {
    std::shared_ptr<Clock> create();
}

#endif
