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
static void ang_SMScb(GSM_Number number, GSM_Message message);
static ang_MessageKindEnum ang_GetMessageKind(GSM_Message msg);

void ANG_init() {
	LOG_entry("ANG_init()");

	if (!CFG_getCfgAng(&ang_cfg)) {
		ang_getInitialConfig(ang_cfg);
	}

	GSM_registerReceiveCB(ang_SMScb);

	LOG_exit("ANG_init()");
}

static void ang_add(char* number, char* message) {
	for (uint8_t i = 1; i < ANG_MAX_NR_ANGELS; i++) {
		if (ang_cfg->angels[i].state == ANG_STATE_EMPTY) {
			ang_cfg->angels[i].state = ANG_STATE_ACTIVE;
			strncpy(ang_cfg->angels[i].message, message, ANG_MESSAGESIZE);
			memcpy(&ang_cfg->angels[i].phonenumber, number, ANG_NUMBERSIZE);
			CFG_persist();
			break;
		}
	}
}

static void ang_remove(char* number) {
	for (uint8_t i = 1; i < ANG_MAX_NR_ANGELS; i++) {
		if (strncmp(ang_cfg->angels[i].phonenumber, number, ANG_NUMBERSIZE) == 0) {
			ang_clearAngel(&ang_cfg->angels[i]);
			CFG_persist();
		}
	}
}

static void ang_passive(char* number) {
	for (uint8_t i = 1; i < ANG_MAX_NR_ANGELS; i++) {
		if (strncmp(ang_cfg->angels[i].phonenumber, number, ANG_NUMBERSIZE) == 0) {
			ang_cfg->angels[i].state = ANG_STATE_PASSIVE;
			CFG_persist();
		}
	}
}

static void ang_active(char* number){
	for (uint8_t i = 1; i < ANG_MAX_NR_ANGELS; i++) {
		if (strncmp(ang_cfg->angels[i].phonenumber, number, ANG_NUMBERSIZE) == 0) {
			ang_cfg->angels[i].state = ANG_STATE_ACTIVE;
			CFG_persist();
		}
	}
}

void ang_clearAngel(ang_AngelStruct* ang)
{
	ang->state = ANG_STATE_EMPTY;
	memset(&ang->phonenumber, 0, ANG_NUMBERSIZE + 1);
	memset(&ang->message, 0, ANG_MESSAGESIZE + 1);
}

void ang_getInitialConfig(ANG_config* cfg)
{
	for (uint8_t i = 1; i < ANG_MAX_NR_ANGELS; i++) {
		ang_clearAngel(&ang_cfg->angels[i]);
	}
	CFG_persist();
}


static void ang_SMScb(GSM_Number number, GSM_Message message)
{
	LOG_entry("ang_SMScb()");
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
	LOG_exit("ang_SMScb()");
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

