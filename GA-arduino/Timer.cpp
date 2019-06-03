/*
 * Timer.c
 *
 *  Created on: 30 apr. 2019
 *      Author: wilbert
 */

#include "Timer.h"
#include "ID.h"

#include <TimerOne.h>

static void tmr_msCB(void);

typedef struct {
	ID_id id;
	uint16_t period;
	TMR_cb cb;
	void* context;
	uint16_t periodCounter;
} tmr_callbackStruct;

static tmr_callbackStruct tmr_callbacks[TMR_MAX_NR_CALLBACKS];
static ID_id tmr_callbackID;

void TMR_init() {
	Serial.println(F("> TMR_init()"));
	for (uint8_t i = 0; i < TMR_MAX_NR_CALLBACKS; i++) {
		tmr_callbacks[i].id = ID_NULL;
		tmr_callbacks[i].period = 0;
		tmr_callbacks[i].cb = NULL;
		tmr_callbacks[i].context = NULL;
		tmr_callbacks[i].periodCounter = 0;
	}
	tmr_callbackID = ID_NULL;

//	Timer1.initialize(1000);
	Timer1.initialize(50);
	Timer1.attachInterrupt(tmr_msCB);
	Serial.println(F("< TMR_init()"));
}

ID_id TMR_registerCB(TMR_cb cb, void* context, uint16_t period) {
	for (uint8_t i = 0; i < TMR_MAX_NR_CALLBACKS; i++) {
		if (tmr_callbacks[i].id == ID_NULL) {
			tmr_callbacks[i].id = ID_getNext(&tmr_callbackID);
			tmr_callbacks[i].cb = cb;
			tmr_callbacks[i].context = context;
			tmr_callbacks[i].period = period;
			return tmr_callbacks[i].id;
		}
	}
	return ID_NULL;
}

static void tmr_msCB(void) {
	for (uint8_t i = 0; i < TMR_MAX_NR_CALLBACKS; i++) {
		if (    tmr_callbacks[i].id != ID_NULL
			 && tmr_callbacks[i].periodCounter > 0) {
			tmr_callbacks[i].periodCounter--;
		}
	}

	for (uint8_t i = 0; i < TMR_MAX_NR_CALLBACKS; i++) {
		if (    tmr_callbacks[i].id != ID_NULL
			 && tmr_callbacks[i].periodCounter == 0) {
			tmr_callbacks[i].periodCounter = tmr_callbacks[i].period;
			tmr_callbacks[i].cb(tmr_callbacks[i].context);
		}
	}
}
