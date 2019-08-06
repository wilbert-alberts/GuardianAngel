#ifndef __MAIN__
#define __MAIN__

#include <Arduino.h>

#include <Print.h>

#include "Config.h"
#include "Timer.h"
#include "Scheduler.h"
#include "Motion.h"
#include "GuardTask.h"
#include "GSM.h"
#include "Alarm.h"
#include "ClockSync.h"
#include "Angel.h"
#include "Log.h"

static void CreateGuardTasks(void);

void setup(void)
{
	TS_timestamp moment;

	Serial.begin(115200);
	LOG("Starting.");
//  CFG_clear();
	CFG_init();

  TMR_init();
	GSM_init();

//	ANG_init();

//	MTN_init();
//	SDL_init();
//	ALM_init();


//	GRD_init();
//	CKS_init();
//	SIM_init();

//	CreateGuardTasks();
	LOG("Started.");
}


void CreateGuardTasks(void)
{
	LOG_entry("CreateGuardTasks");
	static GRD_TaskCfgStruct tskMorning;
	static GRD_TaskCfgStruct tskEvening;
	static GRD_TaskCfgStruct tskAllDay;

	tskMorning.start.hour  = 6;
	tskMorning.start.minute= 0;
	tskMorning.stop.hour  = 11;
	tskMorning.stop.minute= 30;
	tskMorning.minMotion = 1;

	tskEvening.start.hour  = 20;
	tskEvening.start.minute= 0;
	tskEvening.stop.hour  = 1;
	tskEvening.stop.minute= 0;
	tskEvening.minMotion = 1;

	tskAllDay.start.hour  = 7;
	tskAllDay.start.minute= 0;
	tskAllDay.stop.hour  = 1;
	tskAllDay.stop.minute= 0;
	tskAllDay.minMotion = 2;

	GRD_createTask(&tskMorning);
	GRD_createTask(&tskEvening);
	GRD_createTask(&tskAllDay);
	LOG_exit("CreateGuardTasks");
}

void loop(void)
{
	delay(15000);
}
