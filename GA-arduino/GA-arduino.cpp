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

static void Callback(void*);

void setup(void)
{
	TS_timestamp moment;

	Serial.begin(115200);
	LOG("Starting.");
	CFG_init();

	GSM_init();

	TMR_init();
	ANG_init();

	MTN_init();
	SDL_init();
	ALM_init();


	GRD_init();
	CKS_init();
//	SIM_init();
	LOG("Started.");
}


void Callback(void*)
{
	Serial.println(F("1 Second."));
}

void loop(void)
{
	delay(15000);
}


