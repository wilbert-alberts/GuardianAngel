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
#include "Log.h"

static void Callback(void*);

void setup(void)
{
	TS_timestamp moment;

	Serial.begin(115200);
	LOG("Starting.");
	CFG_init();
	CFG_config* cfg = CFG_get();

	GSM_init();

	TMR_init();
//	ANG_init(&cfg->ang_config);

	MTN_init();
	SDL_init();
	ALM_init();


//	GRD_init(&cfg->grd_config);
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


