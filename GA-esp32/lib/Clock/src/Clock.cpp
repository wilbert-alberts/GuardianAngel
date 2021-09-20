/*
 * Clock.cpp
 *
 *  Created on: 14 sep. 2021
 *      Author: wilbert
 */

#include "Clock.hpp"
#include "ClockFactory.hpp"

#include "Time24Factory.hpp"

class ClockImpl : public Clock
{
public:
    ClockImpl();
    virtual ~ClockImpl();
    virtual Time24& getTime() const;
};


Clock* ClockFactory::create() {
    return new ClockImpl();
}

ClockImpl::ClockImpl() {

}

ClockImpl::~ClockImpl() {
    
}

Time24& ClockImpl::getTime() const {
    Time24* now = Time24Factory::create(14,0);    
    return *now;
}