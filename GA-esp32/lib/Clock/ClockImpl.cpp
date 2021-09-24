/*
 * Clock.cpp
 *
 *  Created on: 14 sep. 2021
 *      Author: wilbert
 */

#include <memory>

#include "Clock.hpp"
#include "ClockFactory.hpp"

#include "ActiveTask.hpp"
#include "PeriodicTask.hpp"

#include "Time24Factory.hpp"

class ClockImpl: public Clock, public PeriodicTask {
public:
	ClockImpl();
	virtual ~ClockImpl();
	virtual void terminate();
	virtual void tick();
	virtual std::shared_ptr<Time24> getTime() const;
private:
	std::shared_ptr<Time24> now;
};

ClockImpl::ClockImpl() :
		PeriodicTask("Clock", 1000,4000), now(Time24Factory::create(20, 0, 55)) {
}

ClockImpl::~ClockImpl() {
}

void ClockImpl::terminate() {
	PeriodicTask::terminate();
}
void ClockImpl::tick() {
#ifdef GA_POSIX
	std::cout << "Tick ";
	now->addSeconds(1);
	std::string n(now->toString());
	std::cout << n << std::endl;
#else
	Serial.print("Tick ");
	now->addSeconds(1);
	std::string n(now->toString());
	Serial.println(n.c_str());
#endif
}

std::shared_ptr<Time24> ClockImpl::getTime() const {
	auto r = Time24Factory::create(now->getHours(), now->getMinutes(), now->getSeconds());
	return r;
}

namespace ClockFactory {
std::shared_ptr<Clock> create() {
	return std::shared_ptr<Clock>(new ClockImpl());
}
}
