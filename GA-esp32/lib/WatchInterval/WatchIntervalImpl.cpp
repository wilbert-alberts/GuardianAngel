#include <Time24.hpp>
#include <WatchInterval.hpp>
#include <memory>

class WatchIntervalImpl: public WatchInterval {
public:
	WatchIntervalImpl(const Time24 &start, const Time24 &end);
	virtual ~WatchIntervalImpl() {
	}
	virtual bool startsAt(const Time24 &t) const;
	virtual bool endsAt(const Time24 &t) const;

	virtual const Time24& getStart() const;
	virtual const Time24& getEnd() const;

	virtual bool matches(std::shared_ptr<Time24> start,
			std::shared_ptr<Time24> end) const;

	virtual void progress(std::shared_ptr<Time24> now, int nrActivations);

	virtual IntervalState getState() const;
	virtual void reset();

private:
	const Time24 &start;
	const Time24 &end;
	IntervalState state;
	int nrActivations;
	int nrActivationToBeOk;

	bool insideInterval(const Time24 &now) const;
	bool insideStartAndEnd(const Time24 &now) const;
	bool insideEndAndStart(const Time24 &now) const;
};

namespace WatchIntervalFactory {
std::shared_ptr<WatchInterval> create(const Time24 &start, const Time24 &end) {

	return std::shared_ptr<WatchInterval>(new WatchIntervalImpl(start, end));
}
}

WatchIntervalImpl::WatchIntervalImpl(const Time24 &start, const Time24 &end) :
		start(start), end(end), state(IntervalState::PASSIVE), nrActivations(0), nrActivationToBeOk(
				1) {
}

bool WatchIntervalImpl::startsAt(const Time24 &t) const {
	return start.compareTo(t) == 0;
}

bool WatchIntervalImpl::endsAt(const Time24 &t) const {
	return end.compareTo(t) == 0;
}

const Time24& WatchIntervalImpl::getStart() const {
	return start;
}

const Time24& WatchIntervalImpl::getEnd() const {
	return end;
}

bool WatchIntervalImpl::matches(std::shared_ptr<Time24> start,
		std::shared_ptr<Time24> end) const {
	auto startMatches = this->start.compareTo(*start) == 0;
	auto endMatches = this->end.compareTo(*end) == 0;
	return startMatches && endMatches;
}

IntervalState WatchIntervalImpl::getState() const {
	return state;
}

void WatchIntervalImpl::reset() {
	nrActivations = 0;
	state = IntervalState::PASSIVE;
}

bool WatchIntervalImpl::insideInterval(const Time24 &now) const {
	if (start.compareTo(end) < 0)
		return insideStartAndEnd(now);
	else
		return insideEndAndStart(now);
}

bool WatchIntervalImpl::insideStartAndEnd(const Time24 &now) const {
	if ((now.compareTo(start) >= 0) && (now.compareTo(end) <= 0))
		return true;
	else
		return false;
}

bool WatchIntervalImpl::insideEndAndStart(const Time24 &now) const {
	if ((now.compareTo(start) >= 0) || (now.compareTo(end) <= 0))
		return true;
	else
		return false;
}

void WatchIntervalImpl::progress(std::shared_ptr<Time24> now,
		int activity) {
	if (state != IntervalState::HELPNEEDED) {
		if (insideInterval(*now)) {
			nrActivations+=activity;
		}
		if (insideInterval(*now) && state == IntervalState::PASSIVE) {
			nrActivations = activity;
			state = IntervalState::ACTIVE;
		}
		if (!insideInterval(*now) && state == IntervalState::ACTIVE) {
			if (nrActivations < nrActivationToBeOk) {
				state = IntervalState::HELPNEEDED;
			} else {
				state = IntervalState::PASSIVE;
			}
			nrActivations = 0;
		}
	}
 }

