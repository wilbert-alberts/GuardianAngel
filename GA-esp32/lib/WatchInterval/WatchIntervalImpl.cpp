#include <memory>

#include "WatchInterval.hpp"
#include "Time24.hpp"
#include "Time24Interval.hpp"
#include "WatchInterval.hpp"
#include "WatchIntervalFactory.hpp"
#include "AlarmHandler.hpp"

class WatchIntervalImpl: public WatchInterval {
public:
	WatchIntervalImpl(std::shared_ptr<AlarmHandler> angel, std::shared_ptr<Time24Interval> iv);
	virtual ~WatchIntervalImpl() {
	}
    virtual bool matches(std::shared_ptr<Time24Interval> other);
	virtual void advanceTime(std::shared_ptr<Time24> now);
	virtual void activityDetected(std::shared_ptr<Time24> now);
	virtual void raiseAlarmIfNeeded();

private:
	bool active;
	int nrActivations;
	int nrActivationToBeOk;
	std::shared_ptr<AlarmHandler> ah;
	std::shared_ptr<Time24Interval> interval;

};

namespace WatchIntervalFactory {
std::shared_ptr<WatchInterval> create(std::shared_ptr<AlarmHandler> ah, std::shared_ptr<Time24Interval> iv) {
	return std::shared_ptr<WatchInterval>(new WatchIntervalImpl(ah, iv));
}
}

WatchIntervalImpl::WatchIntervalImpl(std::shared_ptr<AlarmHandler> a, std::shared_ptr<Time24Interval> iv) :
		active(false), nrActivations(0), nrActivationToBeOk(1), ah(a), interval(iv) {
}

bool WatchIntervalImpl::matches(std::shared_ptr<Time24Interval> other) {
	return interval == other;
}

void WatchIntervalImpl::advanceTime(std::shared_ptr<Time24> now) {
	if (interval != nullptr) {
		if (interval->insideInterval(*now) && !active) {
			nrActivations = 0;
			active = true;
		}
		if (!interval->insideInterval(*now) && active) {
			raiseAlarmIfNeeded();
			nrActivations = 0;
			active = false;
		}
	}
}
void WatchIntervalImpl::activityDetected(std::shared_ptr<Time24> now) {
	if (interval != nullptr) {
		if (interval->insideInterval(*now)) {
			nrActivations++;
		}
	}
}

void WatchIntervalImpl::raiseAlarmIfNeeded() {
	if (nrActivations < nrActivationToBeOk) {
		ah->raiseAlarm();
	}
}

