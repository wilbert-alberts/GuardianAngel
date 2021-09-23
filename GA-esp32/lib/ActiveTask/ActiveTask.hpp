
#include <Arduino.h>

#ifndef ACTIVETASK_HPP_
#define ACTIVETASK_HPP_

class ActiveTask
{
public:
    ActiveTask(const char *taskName, int stackSize = 1000)
    {
        xTaskCreate(
            ActiveTask::_task,
            taskName,
            stackSize,
            this,
            1,
            &taskHandle);
    }
    virtual ~ActiveTask()
    {
        vTaskDelete(taskHandle);
    }

protected:
    virtual void task() {}

private:
    static void _task(void *obj);
    TaskHandle_t taskHandle;
};

#endif

