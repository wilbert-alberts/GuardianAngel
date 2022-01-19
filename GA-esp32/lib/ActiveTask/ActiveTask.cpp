#include "platform.hpp"

#include "ActiveTask.hpp"

ActiveTask::ActiveTask(const char *tn, int ss):
taskName(tn), stackSize(ss)
{
    LOG("> ActiveTask::ActiveTask()");

    LOG("< ActiveTask::ActiveTask()");
}

ActiveTask::~ActiveTask()
{

}

void ActiveTask::startTask() 
{
    LOG("> ActiveTask::startTask()");
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
    LOG("< ActiveTask::startTask()");
}

void ActiveTask::endTask() 
{
    LOG("> ActiveTask::endTask()");
#ifdef GA_POSIX
    pthread_kill(thread, SIGTERM);
#else
    vTaskDelete(taskHandle);
#endif
    LOG("< ActiveTask::endTask()");
}

#ifdef GA_POSIX
void *ActiveTask::_task(void *p)
#else
void ActiveTask::_task(void *p)
#endif
{
    auto obj = static_cast<ActiveTask *>(p);

    obj->task();

    while(1);
#ifdef GA_POSIX
    return nullptr;
#endif
}
