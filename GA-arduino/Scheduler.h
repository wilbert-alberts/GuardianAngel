/*
 * Scheduler.h
 *
 *  Created on: 22 apr. 2019
 *      Author: wilbert
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#define SDL_MAX_NR_TASKS (8)

#include <stdint.h>
#include "Timestamp.h"
#include "ID.h"

typedef void (*SDL_taskFunc)(void* context);

void  SDL_init();
ID_id SDL_addTask(const TS_timestamp* moment, SDL_taskFunc task, void* context);
void  SDL_removeTask(ID_id task);
void  SDL_getTime(TS_timestamp* dest);
uint8_t  SDL_setTime(const TS_timestamp* dest, uint8_t seconds);


#endif /* SCHEDULER_H_ */
