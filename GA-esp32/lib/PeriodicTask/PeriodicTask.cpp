#include "PeriodicTask.hpp"

PeriodicTask::PeriodicTask(const char *taskName, int periodInMs, int stackSize)
:    ActiveTask(taskName, stackSize)
,    period(periodInMs)
{
}

void PeriodicTask::task()
{
    auto now = xTaskGetTickCount();
    while (1)
    {
        tick();
        vTaskDelayUntil(&now, pdMS_TO_TICKS(period));
    }
}