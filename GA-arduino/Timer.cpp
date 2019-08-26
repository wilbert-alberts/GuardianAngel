/*
 * Timer.c
 *
 *  Created on: 30 apr. 2019
 *      Author: wilbert
 */

#include "Timer.h"
#include "ID.h"
#include "Log.h"

#include <TimerOne.h>

static void tmr_msCB(void);

typedef struct {
	ID_id id;
	uint16_t period;
	TMR_cb cb;
	void *context;
	uint16_t periodCounter;
} tmr_callbackStruct;

static tmr_callbackStruct tmr_callbacks[TMR_MAX_NR_CALLBACKS];
static ID_id tmr_callbackID;

void TMR_init() {
	LOG_entry("TMR_init");
	for (uint8_t i = 0; i < TMR_MAX_NR_CALLBACKS; i++) {
		tmr_callbacks[i].id = ID_NULL;
		tmr_callbacks[i].period = 0;
		tmr_callbacks[i].cb = NULL;
		tmr_callbacks[i].context = NULL;
		tmr_callbacks[i].periodCounter = 0;
	}
	tmr_callbackID = ID_NULL;

	Timer1.initialize(1000);
	Timer1.attachInterrupt(tmr_msCB);
	LOG_exit("TMR_init");
}

ID_id TMR_registerCB(TMR_cb cb, void *context, uint16_t period) {
//	LOG_entry("TMR_registerCB");
//	LOG_noCR("period: ");
//	LOG_nf(period);
//	LOG_nf("\n");
	for (uint8_t i = 0; i < TMR_MAX_NR_CALLBACKS; i++) {
		if (tmr_callbacks[i].id == ID_NULL) {
			tmr_callbacks[i].id = ID_getNext(&tmr_callbackID);
			tmr_callbacks[i].periodCounter = period;
			tmr_callbacks[i].cb = cb;
			tmr_callbacks[i].context = context;
			tmr_callbacks[i].period = period;

			LOG_noCR("id: ");
			LOG_nf(tmr_callbacks[i].id);
//			LOG_nf("\n");
//			LOG_exit("TMR_registerCB");
			return tmr_callbacks[i].id;
		}
	}
//	LOG("TMR_registerCB(): unable to register cb, out of free slots.");
//	LOG_exit("TMR_registerCB");
	return ID_NULL;
}

void TMR_checkPendingCBs()
{
	for (uint8_t i = 0; i < TMR_MAX_NR_CALLBACKS; i++) {
		if (tmr_callbacks[i].id != ID_NULL
				&& tmr_callbacks[i].periodCounter == 0) {
			noInterrupts();
			tmr_callbacks[i].periodCounter = tmr_callbacks[i].period;
			interrupts();
			tmr_callbacks[i].cb(tmr_callbacks[i].context);
		}
	}
}

static void tmr_msCB(void) {
	for (uint8_t i = 0; i < TMR_MAX_NR_CALLBACKS; i++) {
		if (tmr_callbacks[i].id != ID_NULL
				&& tmr_callbacks[i].periodCounter > 0) {
			tmr_callbacks[i].periodCounter--;
		}
	}

}
