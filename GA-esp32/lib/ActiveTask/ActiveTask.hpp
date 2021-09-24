

#ifndef ACTIVETASK_HPP_
#define ACTIVETASK_HPP_

#include "platform.hpp"

class ActiveTask
{
public:
    ActiveTask(const char *taskName, int stackSize = 1000);
    virtual ~ActiveTask();

protected:
    virtual void task() = 0;

private:
    static void* _task(void *obj);

#ifdef GA_POSIX
    pthread_t thread;
#else
    TaskHandle_t taskHandle;
#endif
};

#endif

