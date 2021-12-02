/*
 * ActivityDetectorImpl.cpp
 *
 *  Created on: 24 sep. 2021
 *      Author: wilbert
 */

#include <ActivityDetectorImpl.hpp>
#include <memory>
#include <PeriodicTask.hpp>

class IActivityDetector;

ActivityDetectorImpl::ActivityDetectorImpl(ValueProvider vp) :
		nrActivations(0), activityDebouncer(5, [=]() -> int {
			return vp();
		}) {

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

PeriodicTask* createTask(ValueProvider vp) {
	auto ad = std::shared_ptr<ActivityDetectorImpl>(new ActivityDetectorImpl(vp));
	auto r = new PeriodicTask("ActivityDetector", ad, 50, 4000);
	return r;
}


}

