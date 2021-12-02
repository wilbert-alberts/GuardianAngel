
#ifndef PERIODICTASK_HPP_
#define PERIODICTASK_HPP_

#include <ActiveTask.hpp>
#include <memory>

class ITicking;

class PeriodicTask : public ActiveTask
{
public:
    PeriodicTask(const char *taskName, std::shared_ptr<ITicking> ticker, int periodInMs, int stackSize = 1000);
    virtual ~PeriodicTask()
    {
    }
    virtual void task();

private:
    std::shared_ptr<ITicking> ticker;
    int period;
    bool terminateRequested;
};

#endif
