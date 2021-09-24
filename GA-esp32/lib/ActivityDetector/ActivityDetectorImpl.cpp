/*
 * ActivityDetectorImpl.cpp
 *
 *  Created on: 24 sep. 2021
 *      Author: wilbert
 */

#include <memory>
#include "ActivityDetector.hpp"
#include "ActivityDetectorFactory.hpp"
#include "Debouncer.hpp"
#include "PeriodicTask.hpp"

class ActivityDetectorImpl: public PeriodicTask, public ActivityDetector {
public:
	ActivityDetectorImpl(ValueProvider vp);
	virtual ~ActivityDetectorImpl() {
	}
	virtual void tick();
	virtual void setListener(std::shared_ptr<ActivityListener> listener);

private:
	// Handle detected
	void activityDetected();

	Debouncer activityDebouncer;
//	ValueProvider valueProvider;

	std::shared_ptr<ActivityListener> listener;
};

ActivityDetectorImpl::ActivityDetectorImpl(ValueProvider vp) :
		PeriodicTask("ActivityDetector", 50, 4000), activityDebouncer(5,
				[=]() -> int {
					return vp();
				}) ,listener(nullptr) {

}

//int ActivityDetectorImpl::getRawActivity() {
//	return 1;
//}

void ActivityDetectorImpl::tick() {
	bool changed = activityDebouncer.tick();

	int newValue = activityDebouncer.getValue();
	if (newValue > 0 && changed) {
		activityDetected();
	}

}

void ActivityDetectorImpl::activityDetected() {
	if (listener != nullptr)
		listener->activityDetected();
}

void ActivityDetectorImpl::setListener(std::shared_ptr<ActivityListener> l) {
	l = listener;
}

namespace ActivityDetectorFactory {
std::shared_ptr<ActivityDetector> create(ValueProvider vp) {
	return std::shared_ptr<ActivityDetector>(new ActivityDetectorImpl(vp));
}
}

