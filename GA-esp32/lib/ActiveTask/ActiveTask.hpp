

#ifndef ACTIVETASK_HPP_
#define ACTIVETASK_HPP_

#ifdef GA_POSIX
#include <pthread.h>
#endif

class ActiveTask
{
public:
    ActiveTask(const char *taskName, int stackSize = 1000);
    virtual ~ActiveTask();

protected:
    virtual void task() = 0;

private:
#ifdef GA_POSIX
    pthread_t thread;
    static void* _task(void *obj);
#else
    TaskHandle_t taskHandle;
    static void _task(void *obj);
#endif
};

#endif

