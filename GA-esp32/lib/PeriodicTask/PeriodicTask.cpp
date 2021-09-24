#include "platform.hpp"

#include "PeriodicTask.hpp"

PeriodicTask::PeriodicTask(const char *taskName, int periodInMs, int stackSize) : ActiveTask(taskName, stackSize), period(periodInMs), terminateRequested(
																																		   false)
{
}

void PeriodicTask::terminate()
{
	terminateRequested = true;
}

void PeriodicTask::task()
{
#ifdef GA_POSIX
	while (!terminateRequested)
	{
		tick();
		usleep(1000 * period);
	}
#else
	Serial.println("> PeriodicTask::task()");
	auto now = xTaskGetTickCount();
	while (1)
	{
	    tick();
	    vTaskDelayUntil(&now, pdMS_TO_TICKS(period));
	}
#endif
}
