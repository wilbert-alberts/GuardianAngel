/*
 * Clock.hpp
 *
 *  Created on: 14 sep. 2021
 *      Author: wilbert
 */


#ifndef CLOCK_HPP_
#define CLOCK_HPP_

#include <memory>

#include "Time24.hpp"

class Clock {
public:
    virtual ~Clock() {};
    virtual std::shared_ptr<Time24> getTime() const = 0;
};

#endif
