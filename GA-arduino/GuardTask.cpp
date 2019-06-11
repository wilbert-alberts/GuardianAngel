/*
 * GuardTask.c
 *
 *  Created on: 23 apr. 2019
 *      Author: wilbert
 */
#include <Arduino.h>

#include "GuardTask.h"
#include "Log.h"
#include "Scheduler.h"
#include "ID.h"
#include "Motion.h"
#include "Alarm.h"
#include "Config.h"

typedef enum grd_TaskStateEnum
{
	GRD_TASKSTATE_EMPTY,
	GRD_TASKSTATE_IDLE,
	GRD_TASKSTATE_ACTIVE
} grd_TaskStateEnum;

typedef struct grd_TaskStruct
{
	GRD_TaskCfgStruct* cfg;

	uint8_t           motionsDetected;
	grd_TaskStateEnum state;
	ID_id             schedTaskId;
	ID_id             motionListenerId;
} grd_TaskStruct;

static GRD_config* grd_cfg;

static grd_TaskStruct grd_tasks[GRD_MAX_TASKS];

static void grd_startTask(void* context);
static void grd_stopTask(void* context);
static void grd_motionDetected(void* context);
static TS_timestamp grd_determineNextHHMM(const TS_HHmm* hhmm);
static void grd_clearTaskCfg(GRD_TaskCfgStruct* cfg);
static void grd_clearTask(grd_TaskStruct* task);

void GRD_init()
{
	LOG_entry("GRD_init");

	for (uint8_t i=1; i< GRD_MAX_TASKS; i++) {
		grd_clearTask(&grd_tasks[i]);
	}

	if (!CFG_getCfgGrd(&grd_cfg)) {
		for (uint8_t i=1; i< GRD_MAX_TASKS; i++) {
			grd_clearTaskCfg(grd_tasks[i].cfg);
		}
		CFG_persist();
	}

	for (uint8_t i=1; i< GRD_MAX_TASKS; i++) {
		grd_tasks[i].cfg = &grd_cfg->tasks[i];
	}

	LOG_exit("GRD_init");
}

void GRD_createTask(GRD_TaskCfgStruct* taskCfg)
{
	for (uint8_t i; i<GRD_MAX_TASKS; i++) {
		if (grd_tasks[i].state == GRD_TASKSTATE_EMPTY) {
			// Found empty slot
			*grd_tasks[i].cfg = *taskCfg;
			grd_tasks[i].motionsDetected = 0;
			grd_tasks[i].state = GRD_TASKSTATE_IDLE;

			// Determine start moment
			TS_timestamp start = grd_determineNextHHMM(&taskCfg->start);

			// Schedule start task
			grd_tasks[i].schedTaskId = SDL_addTask(&start, grd_startTask, &grd_tasks[i]);
			return;
		}
	}
	LOG("GRD_createTask(): unable to create task, out of free slots.")
}

static void grd_startTask(void* context)
{
	TS_timestamp now;
	grd_TaskStruct* task = (grd_TaskStruct*)context;

	LOG_entry("grd_startTask");
	SDL_getTime(&now);
	TS_print(&now);

	task->motionsDetected = 0;
	task->state = GRD_TASKSTATE_ACTIVE;
	task->motionListenerId = MTN_addListener(grd_motionDetected, context);

	// Determine moment to stop task
	TS_timestamp stop = grd_determineNextHHMM(&task->cfg->stop);

	task->schedTaskId = SDL_addTask(&stop, grd_stopTask, context);

	LOG_exit("grd_startTask");
}

static void grd_stopTask(void* context)
{
	TS_timestamp now;
	grd_TaskStruct* task = (grd_TaskStruct*)context;

	LOG_entry("grd_stopTask");
	SDL_getTime(&now);
	TS_print(&now);

	// Stop listening.
	MTN_removeListener(task->motionListenerId);

	// Check whether we need to send alarm.
	if (task->motionsDetected < task->cfg->minMotion) {
		ALM_raiseAlarm();
	}

	// Determine moment to restart task
	TS_timestamp start = grd_determineNextHHMM(&task->cfg->start);

	task->schedTaskId = SDL_addTask(&start, grd_startTask, context);
	LOG_exit("grd_stopTask");
}

static void grd_motionDetected(void* context)
{
	grd_TaskStruct* task = (grd_TaskStruct*)context;
	TS_timestamp now;

	LOG_entry("grd_motionDetected");
	SDL_getTime(&now);

	TS_print(&now);	// Increase detected motions, but avoid rollover
	if (task->motionsDetected <255)
		task->motionsDetected++;

	LOG_exit("grd_motionDetected");
}


static TS_timestamp grd_determineNextHHMM(const TS_HHmm* hhmm)
{
	// Find first moment to moment
	TS_timestamp now;
	SDL_getTime(&now);

	TS_timestamp moment = now;
	moment.hour = hhmm->hour;
	moment.minute = hhmm->minute;

	// Test whether we can moment it today or need to moment it tomorrow
	if (TS_compare(&now, &moment) >=0 ) {
		// Start moment has already passed today, moment tomorrow
		TS_addDays(&moment, 1);
	}

	return moment;
}


static void grd_clearTaskCfg(GRD_TaskCfgStruct* cfg)
{
	cfg->start.hour = 0;
	cfg->start.minute = 0;
	cfg->stop = grd_tasks[0].cfg->start;
	cfg->minMotion = 255;
}

static void grd_clearTask(grd_TaskStruct* task)
{
	task->motionsDetected = 0;
	task->state = GRD_TASKSTATE_EMPTY;
	task->schedTaskId= ID_NULL;
	task->motionListenerId= ID_NULL;
	task->cfg = NULL;
}
