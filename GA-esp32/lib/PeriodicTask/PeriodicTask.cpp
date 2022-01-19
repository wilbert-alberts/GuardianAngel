#include <ITicking.hpp>
#include <PeriodicTask.hpp>
#include <unistd.h>

#include <platform.hpp>


PeriodicTask::PeriodicTask(const char *taskName,
		std::shared_ptr<ITicking> _ticker, int periodInMs, int stackSize) :
		ActiveTask(taskName, stackSize), ticker(_ticker), period(periodInMs) {
	LOG("> PeriodicTask()");
	LOG("< PeriodicTask()");
}
 
PeriodicTask::~PeriodicTask()
{
    LOG("> PeriodicTask::~PeriodicTask()");
    LOG("< PeriodicTask::~PeriodicTask()");
}

void PeriodicTask::endTask() {
	LOG("> PeriodicTask::endTask()");
#ifdef GA_POSIX
#else
	xSemaphoreTake(continueRunning,0);
	xSemaphoreTake(terminated,0);
	vSemaphoreDelete(continueRunning);
	vSemaphoreDelete(terminated);
#endif
	ActiveTask::endTask();
	LOG("< PeriodicTask::endTask()");
}

void PeriodicTask::task() {
	// LOG("> PeriodicTask::task()");

#ifdef GA_POSIX

	while (!terminateRequested) {
		ticker->tick();
		usleep(1000 * period);
	}
	terminated = true;

#else
	continueRunning = xSemaphoreCreateBinary();
	terminated      = xSemaphoreCreateBinary();

	xSemaphoreGive(continueRunning);
	auto now = xTaskGetTickCount();
	while (uxSemaphoreGetCount(continueRunning) == 1)
	{
	    ticker->tick();
	    vTaskDelayUntil(&now, pdMS_TO_TICKS(period));
	}
	xSemaphoreGive(terminated);

#endif
	// LOG("< PeriodicTask::task()");
}
