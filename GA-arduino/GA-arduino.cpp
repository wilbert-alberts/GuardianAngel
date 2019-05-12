#include <Arduino.h>

#include <Print.h>

#include "Config.h"
#include "Timer.h"
#include "Scheduler.h"
#include "Motion.h"
#include "GuardTask.h"

void setup(void)
{
	Serial.begin(115200);
	Serial.println("Starting");
	Serial.println("cfg size: ");
	Serial.println(sizeof(CFG_config));
//	CFG_init();
//	CFG_config* cfg = CFG_get();
//	TMR_init();
//	SDL_init();
//	MTN_init();
//	GRD_init(&cfg->grd_config);
//	ANG_init(&cfg->ang_config);
	Serial.println("Started");
}


void loop(void)
{
	//Serial.println("Running");
	delay(5000);
}
