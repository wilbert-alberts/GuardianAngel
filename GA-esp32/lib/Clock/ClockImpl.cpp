/*
 * Clock.cpp
 *
 *  Created on: 14 sep. 2021
 *      Author: wilbert
 */

#include <memory>

#include "platform.hpp"

#include "Clock.hpp"
#include "ClockFactory.hpp"

#include "ActiveTask.hpp"
#include "PeriodicTask.hpp"

#include "Time24Factory.hpp"
#include "Time24.hpp"

#include "GSM.hpp"

class ClockImpl: public Clock, public PeriodicTask {
	const int ALIGN_WITH_GSM_PERIOD_IN_SECS = (60*30); // Every 30 mins
	const int CLOCK_PERIOD_IN_SECS = 1;

public:
	ClockImpl();
	virtual ~ClockImpl();
	virtual void tick();
	virtual std::shared_ptr<Time24> getTime() const;
	virtual void setGSM(std::shared_ptr<GSM> gsm);

private:
	std::shared_ptr<Time24> now;
	std::shared_ptr<GSM> gsm;
	int updateDue;

	void updateNow();
};

ClockImpl::ClockImpl() :
		PeriodicTask("Clock", CLOCK_PERIOD_IN_SECS * 1000, 4000)
, now(Time24Factory::create(20, 0, 55))
, gsm(nullptr)
, updateDue(0) {
}

ClockImpl::~ClockImpl() {
}

void ClockImpl::tick() {
	now->addSeconds(CLOCK_PERIOD_IN_SECS);
	std::string n(now->toString());
	updateNow();

#ifdef GA_POSIX
	std::cout << "Tick ";
	std::cout << n << std::endl;
#else
	Serial.print("Tick ");
	Serial.println(n.c_str());
#endif
}

std::shared_ptr<Time24> ClockImpl::getTime() const {
	auto r = Time24Factory::create(now->getHours(), now->getMinutes(),
			now->getSeconds());
	return r;
}

void ClockImpl::setGSM(std::shared_ptr<GSM> _gsm) {
	gsm = _gsm;
	updateNow();
}

void ClockImpl::updateNow() {
	// Only update if GSM has been set
	if (gsm != nullptr) {
		// Check whether update is due
		if (updateDue == 0) {
			now = gsm->getTime();
			updateDue = ALIGN_WITH_GSM_PERIOD_IN_SECS/CLOCK_PERIOD_IN_SECS;
		} else
			updateDue--;
	}
}

namespace ClockFactory {
std::shared_ptr<Clock> create() {
	return std::shared_ptr<Clock>(new ClockImpl());
}
}

