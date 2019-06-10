/*
 * GuardTask.h
 *
 *  Created on: 23 apr. 2019
 *      Author: wilbert
 */

#ifndef GUARDTASK_H_
#define GUARDTASK_H_

#include "Timestamp.h"

#define GRD_MAX_TASKS (5)

typedef struct
{
	TS_HHmm start;
	TS_HHmm stop;
	uint8_t minMotion;
} GRD_TaskCfgStruct;

typedef struct
{
	GRD_TaskCfgStruct tasks[GRD_MAX_TASKS];
} GRD_config;

void GRD_init();
void GRD_createTask(GRD_TaskCfgStruct* taskCfg);

#endif /* GUARDTASK_H_ */
