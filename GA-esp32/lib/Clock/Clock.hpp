/*
 * Clock.hpp
 *
 *  Created on: 14 sep. 2021
 *      Author: wilbert
 */


#ifndef CLOCK_HPP_
#define CLOCK_HPP_

#include <ITimeProvider.hpp>
#include <memory>

class Time24;
class GSM;

class Clock : public ITimeProvider {
public:
    virtual ~Clock() {};
    virtual std::shared_ptr<Time24> getTime() const = 0;
    virtual void setGSM(std::shared_ptr<GSM> gsm) = 0;
};

#endif
