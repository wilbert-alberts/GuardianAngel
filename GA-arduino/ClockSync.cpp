/*
 * ClockSync.cpp
 *
 *  Created on: Jun 3, 2019
 *      Author: ben
 */

#include "GSM.h"
#include "Scheduler.h"
#include "Log.h"

static void cks_syncTime();

void CKS_init()
{
	LOG_entry("CKS_init");
	cks_syncTime();
	LOG_exit("CKS_init");
}

static void cks_syncTime()
{

	TS_timestamp now;
	uint8_t secs;
	uint8_t r;

	LOG_entry("cks_syncTime");
	do {
		GSM_getTime(&now, &secs);
		r = SDL_setTime(&now, secs);
	} while (r!=0);
	LOG_exit("cks_syncTime");
}

