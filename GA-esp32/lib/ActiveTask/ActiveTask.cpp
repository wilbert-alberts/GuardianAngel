#include "platform.hpp"

#include "ActiveTask.hpp"

ActiveTask::ActiveTask(const char *taskName, int stackSize) {
#ifdef GA_POSIX

	pthread_create(&thread, NULL, ActiveTask::_task, this);

#else

        xTaskCreate(
            ActiveTask::_task,
            taskName,
            stackSize,
            this,
            1,
            &taskHandle);

#endif
}

ActiveTask::~ActiveTask() {
#ifdef GA_POSIX
	pthread_kill(thread, SIGTERM);
#else
	vTaskDelete(taskHandle);
#endif
}

#include "ActiveTask.hpp"
#ifdef GA_POSIX
void* ActiveTask::_task(void *p)
#else
void ActiveTask::_task(void *p)
#endif
		{
	auto obj = static_cast<ActiveTask*>(p);

	obj->task();

#ifdef GA_POSIX
	return nullptr;
#endif
}
