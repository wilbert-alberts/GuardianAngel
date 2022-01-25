#include <ITicking.hpp>
#include <PeriodicTask.hpp>
#include <unistd.h>

#include <platform.hpp>


PeriodicTask::PeriodicTask(const char *taskName,
		std::shared_ptr<ITicking> _ticker, int periodInMs, int stackSize) :
		ActiveTask(taskName, stackSize), ticker(_ticker), period(periodInMs) {
	LOG_ENTRY();
	LOG_EXIT();
}
 
PeriodicTask::~PeriodicTask()
{
	LOG_ENTRY();
	LOG_EXIT();
}

void PeriodicTask::endTask() {
	LOG_ENTRY();
#ifdef GA_POSIX
	sem_wait(&continueRunning);
	sem_wait(&terminated);
	sem_destroy(&continueRunning);
	sem_destroy(&terminated);

#else
	xSemaphoreTake(continueRunning,0);
	xSemaphoreTake(terminated,0);
	vSemaphoreDelete(continueRunning);
	vSemaphoreDelete(terminated);
#endif
	ActiveTask::endTask();
	LOG_EXIT();
}

void PeriodicTask::task() {
	// LOG_ENTRY();

#ifdef GA_POSIX
	sem_init(&continueRunning, 0, 0);
	sem_init(&terminated, 0,0);

	int localContinueRunning;
	sem_post(&continueRunning);
	sem_getvalue(&continueRunning, &localContinueRunning);

	while (localContinueRunning>0) {
		ticker->tick();
		usleep(1000 * period);
		sem_getvalue(&continueRunning, &localContinueRunning);
	}
	sem_post(&terminated);

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
	// LOG_EXIT();
}
