#include <platform.hpp>

#include <Time24.hpp>
#include <WatchInterval.hpp>
#include <memory>

class WatchIntervalImpl : public WatchInterval
{
public:
	WatchIntervalImpl(std::shared_ptr<Time24> start, std::shared_ptr<Time24> end);
	virtual ~WatchIntervalImpl()
	{
	}
	virtual bool startsAt(std::shared_ptr<Time24> now) const;
	virtual bool endsAt(std::shared_ptr<Time24> now) const;

	virtual std::shared_ptr<const Time24> getStart() const;
	virtual std::shared_ptr<const Time24> getEnd() const;

	virtual bool matches(std::shared_ptr<Time24> start,
						 std::shared_ptr<Time24> end) const;

	virtual void timeProgress(std::shared_ptr<Time24> now);
	virtual void activityDetected(int nrActivations = 1);

	virtual IntervalState getState() const;
	virtual void reset();

private:
	std::shared_ptr<Time24> start;
	std::shared_ptr<Time24> end;
	IntervalState state;
	int nrActivations;
	int nrActivationToBeOk;

	bool insideInterval(std::shared_ptr<Time24> now) const;
	bool insideStartAndEnd(std::shared_ptr<Time24> now) const;
	bool insideEndAndStart(std::shared_ptr<Time24> now) const;
};

namespace WatchIntervalFactory
{
	std::shared_ptr<WatchInterval> create(std::shared_ptr<Time24> start, std::shared_ptr<Time24> end)
	{

		return std::shared_ptr<WatchInterval>(new WatchIntervalImpl(start, end));
	}
}

WatchIntervalImpl::WatchIntervalImpl(std::shared_ptr<Time24> start, std::shared_ptr<Time24> end) : start(start), end(end), state(IntervalState::PASSIVE), nrActivations(0), nrActivationToBeOk(
																																												1)
{
	LOG_ENTRY("start: %s, end: %s", start->toString().c_str(), end->toString().c_str());
	LOG_EXIT();
}

bool WatchIntervalImpl::startsAt(std::shared_ptr<Time24> t) const
{
	return start->compareTo(*t) == 0;
}

bool WatchIntervalImpl::endsAt(std::shared_ptr<Time24> t) const
{
	return end->compareTo(*t) == 0;
}

std::shared_ptr<const Time24> WatchIntervalImpl::getStart() const
{
	return start;
}

std::shared_ptr<const Time24> WatchIntervalImpl::getEnd() const
{
	return end;
}

bool WatchIntervalImpl::matches(std::shared_ptr<Time24> _start,
								std::shared_ptr<Time24> _end) const
{
	auto startMatches = start->compareTo(*_start) == 0;
	auto endMatches = end->compareTo(*_end) == 0;
	return startMatches && endMatches;
}

IntervalState WatchIntervalImpl::getState() const
{
	LOG_ENTRY("start: %s, end: %s", this->start->toString().c_str(), this->end->toString().c_str());
	LOG_EXIT("state: %d", this->state);
	return state;
}

void WatchIntervalImpl::reset()
{
	LOG_ENTRY("start: %s, end: %s", start->toString().c_str(), end->toString().c_str());
	nrActivations = 0;
	state = IntervalState::PASSIVE;
	LOG_EXIT();
}

bool WatchIntervalImpl::insideInterval(std::shared_ptr<Time24> now) const
{
	LOG_ENTRY("start: %s, end: %s, now: %s", start->toString().c_str(), end->toString().c_str(), now->toString().c_str());
	bool result = false;
	if (start->compareTo(*end) < 0)
		result = insideStartAndEnd(now);
	else
		result = insideEndAndStart(now);
	LOG_EXIT("result: %s", result ? "T" : "F");
	return result;
}

bool WatchIntervalImpl::insideStartAndEnd(std::shared_ptr<Time24> now) const
{
	LOG_ENTRY();
	bool result = false;
	if ((now->compareTo(*start) >= 0) && (now->compareTo(*end) <= 0))
		result = true;
	else
		result = false;
	LOG_EXIT("result: %s", result ? "T" : "F");
	return result;
}

bool WatchIntervalImpl::insideEndAndStart(std::shared_ptr<Time24> now) const
{
	LOG_ENTRY();
	bool result = false;
	if ((now->compareTo(*start) >= 0) || (now->compareTo(*end) <= 0))
		result = true;
	else
		result = false;
	LOG_EXIT("result: %s", result ? "T" : "F");
	return result;
}

void WatchIntervalImpl::timeProgress(std::shared_ptr<Time24> now)
{
	LOG_ENTRY("start: %s, end: %s, now: %s, state: %d", this->start->toString().c_str(), this->end->toString().c_str(), now->toString().c_str(), this->state);
	if (state != IntervalState::HELPNEEDED)
	{
		if (insideInterval(now) && state == IntervalState::PASSIVE)
		{
			nrActivations = 0;
			state = IntervalState::ACTIVE;
			LOG("ACTIVATED");
		}
		if (!insideInterval(now) && state == IntervalState::ACTIVE)
		{
			if (nrActivations < nrActivationToBeOk)
			{
				state = IntervalState::HELPNEEDED;
				LOG("HELP!!!");
			}
			else
			{
				state = IntervalState::PASSIVE;
				LOG("PASSIVATED");
			}
			nrActivations = 0;
		}
	}
	LOG_EXIT("state: %d", this->state);
}

void WatchIntervalImpl::activityDetected(int activity)
{
	LOG_ENTRY("start: %s, end: %s, activity: %d, state: %d", this->start->toString().c_str(), this->end->toString().c_str(), activity, this->state);
	if (state == IntervalState::ACTIVE)
	{
		nrActivations += activity;
	}
	LOG_EXIT("nrActivations: %d", nrActivations);
}
