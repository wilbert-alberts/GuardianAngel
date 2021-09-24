
#ifndef PERIODICTASK_HPP_
#define PERIODICTASK_HPP_

#include "ActiveTask.hpp"

class PeriodicTask : public ActiveTask
{
public:
    PeriodicTask(const char *taskName, int periodInMs, int stackSize = 1000);
    virtual ~PeriodicTask()
    {
    }
    virtual void terminate();
    virtual void tick() {}
    virtual void task();

private:
    int period;
    bool terminateRequested;
};

#endif
