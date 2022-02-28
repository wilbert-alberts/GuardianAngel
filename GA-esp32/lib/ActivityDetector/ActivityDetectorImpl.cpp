/*
 * ActivityDetectorImpl.cpp
 *
 *  Created on: 24 sep. 2021
 *      Author: wilbert
 */

#include <ActivityDetectorImpl.hpp>
#include <AngelFactory.hpp>
#include <PeriodicTask.hpp>
#include <memory>

class IActivityDetector;

ActivityDetectorImpl::ActivityDetectorImpl() :
		nrActivations(0), activityDebouncer(5, [=]() -> int {
			return digitalRead(22);
		}) {
	LOG_ENTRY();
	pinMode(22, INPUT_PULLDOWN);
	LOG_EXIT();
}

PeriodicTask* ActivityDetectorImpl::createTask() {
	return new PeriodicTask("activityDetector", std::shared_ptr<ITicking>(this),
			10, 4000);
}
void ActivityDetectorImpl::tick() {
	bool changed = activityDebouncer.tick();

	int newValue = activityDebouncer.getValue();
	if (newValue > 0 && changed) {
		nrActivations++;
	}
}

void ActivityDetectorImpl::clearActivity() {
	LOG_ENTRY();
	nrActivations = 0;
	LOG_EXIT();
}

int ActivityDetectorImpl::getNrActivations() {
	LOG_ENTRY();
	LOG_EXIT("nrActivations: %d", nrActivations);
	return nrActivations;
}

namespace ActivityDetectorFactory {

std::shared_ptr<IActivityDetector> create() {
	return std::shared_ptr<IActivityDetector>(new ActivityDetectorImpl());
}

}

