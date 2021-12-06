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

ActivityDetectorImpl::ActivityDetectorImpl(ValueProvider vp) :
		nrActivations(0), activityDebouncer(5, [=]() -> int {
			return vp();
		}) {

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
	nrActivations = 0;
}

int ActivityDetectorImpl::getNrActivations() {
	return nrActivations;
}

namespace ActivityDetectorFactory {

std::shared_ptr<IActivityDetector> create(ValueProvider vp) {
	return std::shared_ptr<IActivityDetector>(new ActivityDetectorImpl(vp));
}

}

