
#include "ActiveTask.hpp"

void ActiveTask::_task(void *p)
{
    auto obj = static_cast<ActiveTask *>(p);
    obj->task();
}