/*
 * Clock.cpp
 *
 *  Created on: 14 sep. 2021
 *      Author: wilbert
 */

#include <Arduino.h>
#include <memory>

#include "Clock.hpp"
#include "ClockFactory.hpp"

#include "ActiveTask.hpp"
#include "PeriodicTask.hpp"

#include "Time24Factory.hpp"

class ClockImpl : public Clock, public PeriodicTask
{
public:
    ClockImpl();
    virtual ~ClockImpl();
    virtual void tick();
    virtual std::shared_ptr<Time24> getTime() const;
private:
    std::shared_ptr<Time24> now;
};

ClockImpl::ClockImpl()
    : PeriodicTask("Clock", 1000)//, now(Time24Factory::create(20,0,0))
{
    now = Time24Factory::create(20,0,0);
}

ClockImpl::~ClockImpl()
{
}

void ClockImpl::tick()
{
    // now->addSeconds(1);
    Serial.println("tick");
    // std::string n(now->toString()); 
    // Serial.println(n.c_str());
}

std::shared_ptr<Time24> ClockImpl::getTime() const {
    auto r = Time24Factory::create(now->getHours(), now->getMinutes(), now->getSeconds());
    return r;
}

Clock *ClockFactory::create()
{
    return new ClockImpl();
}
