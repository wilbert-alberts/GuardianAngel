/*
 * Clock.cpp
 *
 *  Created on: 14 sep. 2021
 *      Author: wilbert
 */

#include <Clock.hpp>
#include <GSM.hpp>
#include <ITicking.hpp>
#include <PeriodicTask.hpp>
#include <Time24.hpp>
#include <Time24Factory.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <platform.hpp>

class ClockImpl : public Clock, public ITicking
{
	const int ALIGN_WITH_GSM_PERIOD_IN_SECS = (60 *30 ); // Every 30 mins

public:
	const int CLOCK_PERIOD_IN_SECS = 1;

	ClockImpl();
	virtual ~ClockImpl();
	virtual void tick();
	virtual std::shared_ptr<Time24> getTime() const;
	virtual void setGSM(std::shared_ptr<GSM> gsm);
	virtual PeriodicTask *createTask();

private:
	std::shared_ptr<Time24> now;
	std::shared_ptr<GSM> gsm;
	int updateDue;

	void updateNow();
};

ClockImpl::ClockImpl() : now(Time24Factory::create(20, 0, 55)), gsm(nullptr), updateDue(0)
{
	LOG_ENTRY();
	LOG_EXIT();
}

ClockImpl::~ClockImpl()
{
	LOG_ENTRY();
	LOG_EXIT();
}

PeriodicTask *ClockImpl::createTask()
{
	return new PeriodicTask("clock", std::shared_ptr<ITicking>(this), 1000, 4000);
}

void ClockImpl::tick()
{
	now->addSeconds(CLOCK_PERIOD_IN_SECS);
	std::string n(now->toString());
	updateNow();

#ifdef GA_POSIX
	// LOG("Tick ");
	std::cout << n << std::endl;
#else
	// LOG("Tick ");
	// ESP_LOGD("GA","%s", n.c_str());
#endif
}

std::shared_ptr<Time24> ClockImpl::getTime() const
{
	LOG_ENTRY();
	auto r = Time24Factory::create(now->getHours(), now->getMinutes(),
								   now->getSeconds());
	LOG_EXIT("result: %s", r->toString().c_str());
	return r;
}

void ClockImpl::setGSM(std::shared_ptr<GSM> _gsm)
{
	LOG_ENTRY();
	gsm = _gsm;
	updateDue = 0;
	updateNow();
	LOG_EXIT();
}

void ClockImpl::updateNow()
{
	LOG_ENTRY("gsm is set: %s, updateDue: %d (0 means: update)", boolToString(gsm!=nullptr), updateDue);
	// Only update if GSM has been set
	if (gsm != nullptr)
	{
		// Check whether update is due
		if (updateDue == 0)
		{
			now = gsm->getTime();
			updateDue = ALIGN_WITH_GSM_PERIOD_IN_SECS / CLOCK_PERIOD_IN_SECS;
		}
		else
			updateDue--;
	}
	LOG_EXIT("now: %s, updateDue: %d", now->toString().c_str(), updateDue);
}

namespace ClockFactory
{
	std::shared_ptr<Clock> create()
	{
		return std::shared_ptr<Clock>(new ClockImpl());
	}

}
