/*
 * ActivityDetectorImpl.cpp
 *
 *  Created on: 24 sep. 2021
 *      Author: wilbert
 */

#include <ActivityDetectorImpl.hpp>
#include <ValueProvider.hpp>
#include <memory>

class IActivityDetector;

ActivityDetectorImpl::ActivityDetectorImpl(ValueProvider vp) :
		nrActivations(0), activityDebouncer(5, [=]() -> int {
			return vp();
		}) {

}

void ActivityDetectorImpl::doTick() {
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

