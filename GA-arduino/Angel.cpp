/*
 * Angel.h
 *
 *  Created on: 22 apr. 2019
 *      Author: wilbert
 */

#include <string.h>
#include <stdint.h>

#include "Angel.h"
#include "GSM.h"
#include "Log.h"
#include "Config.h"


typedef enum
{
	MSG_UNSUBSCRIBE,
	MSG_ACTIVE,
	MSG_PASSIVE,
	MSG_SUBSCRIBE
} ang_MessageKindEnum;

static char ang_Msg_Unsubscribe[] = "Off";
static char ang_Msg_Active[]      = "Active";
static char ang_Msg_Passive[]     = "Passive";

static ANG_config* ang_cfg;

static void ang_getInitialConfig(ANG_config* cfg);
static void ang_add(char* number, char* message);
static void ang_remove(char* number);
static void ang_passive(char* number);
static void ang_active(char* number);
static void ang_clearAngel(ang_AngelStruct* ang);
static void ang_alarmAngel(ang_AngelStruct* ang);

static void ang_SMScb(GSM_Number number, GSM_Message message);
static ang_MessageKindEnum ang_GetMessageKind(GSM_Message msg);

void ANG_init() {
	LOG_entry("ANG_init");

	if (!CFG_getCfgAng(&ang_cfg)) {
		ang_getInitialConfig(ang_cfg);
	}

	GSM_registerReceiveCB(ang_SMScb);

	LOG_exit("ANG_init");
}

void ANG_alarmAll() {
	LOG_entry("ANG_alarmAll");

	for (uint8_t i = 1; i < ANG_MAX_NR_ANGELS; i++) {
		if (ang_cfg->angels[i].state == ANG_STATE_ACTIVE) {
			ang_alarmAngel(&ang_cfg->angels[i]);
		}
	}


	LOG_exit("ANG_alarmAll");
}

static void ang_add(char* number, char* message) {
	LOG_entry("ang_add");
	LOG_nf(F("ang_add(), angel: "));
	LOG_nf(number);
	LOG_nf("\n");

	// First try replace existing angel
	for (uint8_t i = 1; i < ANG_MAX_NR_ANGELS; i++) {
		if (strncasecmp(ang_cfg->angels[i].phonenumber, number, ANG_NUMBERSIZE) == 0) {
			ang_cfg->angels[i].state = ANG_STATE_ACTIVE;
			strncpy(ang_cfg->angels[i].message, message, ANG_MESSAGESIZE);
			CFG_persist();
			LOG_exit("ang_add");
			return;
		}
	}

	// New angel, try to store
	for (uint8_t i = 1; i < ANG_MAX_NR_ANGELS; i++) {
		if (ang_cfg->angels[i].state == ANG_STATE_EMPTY) {
			ang_cfg->angels[i].state = ANG_STATE_ACTIVE;
			strncpy(ang_cfg->angels[i].message, message, ANG_MESSAGESIZE);
			memcpy(&ang_cfg->angels[i].phonenumber, number, ANG_NUMBERSIZE);
			CFG_persist();
			LOG_exit("ang_add");
			return;
		}
	}

	// No free entry found
	LOG("ang_add(): unable to add new angel, out of slots.")
	LOG_exit("ang_add");
}

static void ang_remove(char* number) {
	LOG_entry("ang_remove");
	LOG_nf(F("ang_remove(), angel: "));
	LOG_nf(number);
	LOG_nf("\n");

	for (uint8_t i = 1; i < ANG_MAX_NR_ANGELS; i++) {
		if (strncmp(ang_cfg->angels[i].phonenumber, number, ANG_NUMBERSIZE) == 0) {
			LOG_nf(F("ang_remove(), found: remove it.\n"));
			ang_clearAngel(&ang_cfg->angels[i]);
			CFG_persist();
		}
	}
	LOG_exit("ang_remove");
}

static void ang_passive(char* number) {
	LOG_entry("ang_passive");
	LOG_nf(F("ang_passive(), angel: "));
	LOG_nf(number);
	LOG_nf("\n");

	for (uint8_t i = 1; i < ANG_MAX_NR_ANGELS; i++) {
		if (strncmp(ang_cfg->angels[i].phonenumber, number, ANG_NUMBERSIZE) == 0) {
			LOG_nf(F("ang_passive(), found: set to passive.\n"));
			ang_cfg->angels[i].state = ANG_STATE_PASSIVE;
			CFG_persist();
		}
	}
	LOG_exit("ang_passive");
}

static void ang_active(char* number){
	LOG_entry("ang_active");
	LOG_nf(F("ang_active(), angel: "));
	LOG_nf(number);
	LOG_nf("\n");
	for (uint8_t i = 1; i < ANG_MAX_NR_ANGELS; i++) {
		if (strncmp(ang_cfg->angels[i].phonenumber, number, ANG_NUMBERSIZE) == 0) {
			LOG_nf(F("ang_active(), found: set to passive.\n"));
			ang_cfg->angels[i].state = ANG_STATE_ACTIVE;
			CFG_persist();
		}
	}
	LOG_exit("ang_active");
}

static void ang_clearAngel(ang_AngelStruct* ang)
{
	ang->state = ANG_STATE_EMPTY;
	memset(&ang->phonenumber, 0, ANG_NUMBERSIZE + 1);
	memset(&ang->message, 0, ANG_MESSAGESIZE + 1);
}

static void ang_getInitialConfig(ANG_config* cfg)
{
	for (uint8_t i = 1; i < ANG_MAX_NR_ANGELS; i++) {
		ang_clearAngel(&ang_cfg->angels[i]);
	}
	CFG_persist();
}


static void ang_SMScb(GSM_Number number, GSM_Message message)
{
	LOG_entry("ang_SMScb");
	ang_MessageKindEnum msgKind = ang_GetMessageKind(message);

	switch(msgKind) {
	case MSG_UNSUBSCRIBE:
		ang_remove(number);
		break;
	case MSG_PASSIVE:
		ang_passive(number);
		break;
	case MSG_ACTIVE:
		ang_active(number);
		break;
	default:
		// Default behaviour is to subscribe
		ang_add(number, message);
		break;
	}
	LOG_exit("ang_SMScb");
}

static ang_MessageKindEnum ang_GetMessageKind(GSM_Message msg)
{
	if (strncasecmp(msg, ang_Msg_Unsubscribe, strlen(ang_Msg_Unsubscribe) == 0))
		return MSG_UNSUBSCRIBE;
	if (strncasecmp(msg, ang_Msg_Passive, strlen(ang_Msg_Passive) == 0))
		return MSG_PASSIVE;
	if (strncasecmp(msg, ang_Msg_Active, strlen(ang_Msg_Active) == 0))
		return MSG_ACTIVE;
	return MSG_SUBSCRIBE;
}

static void ang_alarmAngel(ang_AngelStruct* ang)
{
	LOG_entry("ang_alarmAngel");
	LOG_nf(F("ang_alarmAngel(), angel: "));
	LOG_nf(ang->phonenumber);
	LOG_nf("\n");
	GSM_sendSMS(ang->phonenumber, ang->message);
	LOG_exit("ang_alarmAngel");
}
