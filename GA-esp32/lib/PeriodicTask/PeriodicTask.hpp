
#ifndef PERIODICTASK_HPP_
#define PERIODICTASK_HPP_

#include <ActiveTask.hpp>
#include <memory>
#include <platform.hpp>

class ITicking;

class PeriodicTask : public ActiveTask
{
public:
    PeriodicTask(const char *taskName, std::shared_ptr<ITicking> ticker, int periodInMs, int stackSize = 1000);
    virtual ~PeriodicTask();

    virtual void task();
    virtual void endTask();

private:
    std::shared_ptr<ITicking> ticker;
    int period;

#ifdef GA_POSIX
    sem_t continueRunning;
    sem_t terminated;
#else
    SemaphoreHandle_t continueRunning;
    SemaphoreHandle_t terminated;
#endif
};

#endif
