/*
 * HelpButton.cpp
 *
 *  Created on: 5 dec. 2021
 *      Author: wilbert
 */

#include "platform.hpp"
#include <HelpButton.hpp>
#include <PeriodicTask.hpp>
#include <algorithm>
#include <iterator>

HelpButton::HelpButton() : activityDebouncer(5, [=]() -> int
															 { return digitalRead(21); })
{
	LOG_ENTRY();
	pinMode(21, INPUT_PULLUP);
	LOG_EXIT();
}

HelpButton::~HelpButton()
{
	LOG_ENTRY();
	LOG_EXIT();
}

PeriodicTask *HelpButton::createTask()
{
	return new PeriodicTask("helpButton", std::shared_ptr<ITicking>(this), 10, 4000);
}

void HelpButton::addListener(std::shared_ptr<IBtnListener> listener)
{
	LOG_ENTRY("nrListeners: %d", listeners.size());
	listeners.push_back(listener);
	LOG_EXIT("nrListeners: %d", listeners.size());
}

void HelpButton::delListener(std::shared_ptr<IBtnListener> listener)
{
	LOG_ENTRY("nrListeners: %d", listeners.size());
	auto newEnd = std::remove_if(listeners.begin(), listeners.end(),
								 [&](std::shared_ptr<IBtnListener> el)
								 {
									 return el == listener;
								 });
	listeners.erase(newEnd, listeners.end());
	LOG_EXIT("nrListeners: %d", listeners.size());
}

void HelpButton::notifyListeners()
{
	LOG_ENTRY("nrListeners: %d", listeners.size());
	std::for_each(listeners.begin(), listeners.end(), [](std::shared_ptr<IBtnListener> l)
				  { l->btnPressed(); });
	LOG_EXIT();
}

void HelpButton::tick()
{
	bool changed = activityDebouncer.tick();
	if (changed && activityDebouncer.getValue() == 0)
	{
		notifyListeners();
	}
}
