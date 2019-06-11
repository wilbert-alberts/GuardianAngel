/*
 * GuardTask_cfg.h
 *
 *  Created on: 11 jun. 2019
 *      Author: wilbert
 */

#ifndef GUARDTASK_CFG_H_
#define GUARDTASK_CFG_H_

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


#endif /* GUARDTASK_CFG_H_ */
