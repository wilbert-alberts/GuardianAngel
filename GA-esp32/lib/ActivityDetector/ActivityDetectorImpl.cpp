/*
 * ActivityDetectorImpl.cpp
 *
 *  Created on: 24 sep. 2021
 *      Author: wilbert
 */

#include <memory>
#include <vector>
#include <algorithm>

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
	virtual void addListener(std::shared_ptr<ActivityListener> listener);
	virtual void delListener(std::shared_ptr<ActivityListener> listener);

private:
	// Handle detected
	void activityDetected();

	Debouncer activityDebouncer;
//	ValueProvider valueProvider;

	std::vector<std::shared_ptr<ActivityListener>> listeners;
};

ActivityDetectorImpl::ActivityDetectorImpl(ValueProvider vp) :
		PeriodicTask("ActivityDetector", 50, 4000), activityDebouncer(5,
				[=]() -> int {
					return vp();
				}) {

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
	std::for_each(listeners.begin(), listeners.end(), [](std::shared_ptr<ActivityListener> l){
		l->activityDetected();
	});
}

void ActivityDetectorImpl::addListener(std::shared_ptr<ActivityListener> l) {
	if (l.get() != nullptr) {
		if (std::find(listeners.begin(), listeners.end(), l) == listeners.end()) {
			listeners.push_back(l);
		}
	}
}

void ActivityDetectorImpl::delListener(std::shared_ptr<ActivityListener> l) {
	auto newEnd = std::remove_if(listeners.begin(), listeners.end(),
			[&](std::shared_ptr<ActivityListener> o){
		return o == l;
	});
	listeners.erase(newEnd, listeners.end());
}

namespace ActivityDetectorFactory {
std::shared_ptr<ActivityDetector> create(ValueProvider vp) {
	return std::shared_ptr<ActivityDetector>(new ActivityDetectorImpl(vp));
}
}

