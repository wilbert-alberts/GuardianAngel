#include <Arduino.h>

#include <Print.h>

#include "Config.h"
#include "Timer.h"
#include "Scheduler.h"
#include "Motion.h"
#include "GuardTask.h"

static void Callback(void*);

void setup(void)
{
	TS_timestamp moment;

	Serial.begin(115200);
	Serial.println("Starting\n");
	Serial.print("cfg size: ");
	Serial.println(sizeof(CFG_config));
	CFG_init();
	CFG_config* cfg = CFG_get();
	TMR_init();
	SDL_init();
	MTN_init();
//	GRD_init(&cfg->grd_config);
//	ANG_init(&cfg->ang_config);
	Serial.println("\nStarted");
}


void Callback(void*)
{
	Serial.println("1 Second.");
}

void loop(void)
{
	delay(15000);
}


