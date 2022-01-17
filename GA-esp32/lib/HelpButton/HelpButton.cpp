/*
 * HelpButton.cpp
 *
 *  Created on: 5 dec. 2021
 *      Author: wilbert
 */

#include <HelpButton.hpp>
#include <PeriodicTask.hpp>
#include <algorithm>
#include <iterator>

HelpButton::HelpButton(ValueProvider vp) :
		activityDebouncer(5, [=]() -> int {
			return vp();
		}) {

}

HelpButton::~HelpButton() {

}

PeriodicTask* HelpButton::createTask() {
	return new PeriodicTask("helpButton", std::shared_ptr<ITicking>(this), 10, 4000);
}

void HelpButton::addListener(std::shared_ptr<IBtnListener> listener) {
	listeners.push_back(listener);
}

void HelpButton::delListener(std::shared_ptr<IBtnListener> listener) {
	auto newEnd = std::remove_if(listeners.begin(), listeners.end(),
			[&](std::shared_ptr<IBtnListener> el) {
				return el == listener;
			});
	listeners.erase(newEnd, listeners.end());
}

void HelpButton::notifyListeners() {
	std::for_each(listeners.begin(), listeners.end(), [](std::shared_ptr<IBtnListener> l) {
		l->btnPressed();
	});
}

void HelpButton::tick() {
	bool changed = activityDebouncer.tick();
	if (changed) {
		notifyListeners();
	}
}
