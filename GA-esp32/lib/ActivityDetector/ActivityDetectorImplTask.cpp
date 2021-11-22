/*
 * ActivityDetectorImpl.cpp
 *
 *  Created on: 24 sep. 2021
 *      Author: wilbert
 */

#include <ActivityDetectorImpl.hpp>
#include <ValueProvider.hpp>
#include <memory>

class ActivityDetectorTaskImpl: public PeriodicTask, public ActivityDetectorImpl {
public:
	ActivityDetectorTaskImpl(ValueProvider vp);
	virtual ~ActivityDetectorTaskImpl() {
	}
	virtual void tick();

};

ActivityDetectorTaskImpl::ActivityDetectorTaskImpl(ValueProvider vp) :
		PeriodicTask("ActivityDetector", 50, 4000), ActivityDetectorImpl(vp)
{}

void ActivityDetectorTaskImpl::tick() {
	doTick();
}

namespace ActivityDetectorFactory {
std::shared_ptr<IActivityDetector> createTask(ValueProvider vp) {
	return std::shared_ptr<IActivityDetector>(new ActivityDetectorTaskImpl(vp));
}
}
