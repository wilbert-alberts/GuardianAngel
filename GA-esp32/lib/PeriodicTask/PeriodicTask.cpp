#include <ITicking.hpp>
#include <PeriodicTask.hpp>
#include <unistd.h>

PeriodicTask::PeriodicTask(const char *taskName,
		std::shared_ptr<ITicking> _ticker, int periodInMs, int stackSize) :
		ActiveTask(taskName, stackSize), ticker(_ticker), period(periodInMs), terminateRequested(
				false) {
}

void PeriodicTask::task() {
#ifdef GA_POSIX
	while (1) {
		ticker->tick();
		usleep(1000 * period);
	}
#else
	Serial.println("> PeriodicTask::task()");
	auto now = xTaskGetTickCount();
	while (1)
	{
	    ticker->tick();
	    vTaskDelayUntil(&now, pdMS_TO_TICKS(period));
	}
#endif
}
