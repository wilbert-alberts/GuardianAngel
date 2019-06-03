/*
 * ClockSync.cpp
 *
 *  Created on: Jun 3, 2019
 *      Author: ben
 */

#include "GSM.h"
#include "Scheduler.h"

static void cks_syncTime();

void CKS_init()
{
	cks_syncTime();
}

static void cks_syncTime()
{
	TS_timestamp now;
	uint8_t secs;
	uint8_t r;

	do {
		GSM_getTime(&now, &secs);
		r = SDL_setTime(&now, secs);
	} while (r!=0);

}

