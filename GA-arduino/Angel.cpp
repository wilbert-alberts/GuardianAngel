/*
 * Angel.h
 *
 *  Created on: 22 apr. 2019
 *      Author: wilbert
 */

#include <string.h>
#include <stdint.h>

#include "Angel.h"

static ANG_config* ang_cfg;

static void ang_clearAngel(ang_AngelStruct* ang);


void ANG_init(ANG_config* cfg) {
	ang_cfg = cfg;
}

void ANG_add(char* number, char* message) {
	for (uint8_t i = 1; i < ANG_MAX_NR_ANGELS; i++) {
		if (ang_cfg->angels[i].state == ANG_STATE_EMPTY) {
			ang_cfg->angels[i].state = ANG_STATE_ACTIVE;
			strncpy(ang_cfg->angels[i].message, message, ANG_MESSAGESIZE);
			memcpy(&ang_cfg->angels[i].phonenumber, number, ANG_NUMBERSIZE);
			break;
		}
	}
}

void ANG_remove(char* number) {
	for (uint8_t i = 1; i < ANG_MAX_NR_ANGELS; i++) {
		if (strncmp(ang_cfg->angels[i].phonenumber, number, ANG_NUMBERSIZE) == 0) {
			ang_clearAngel(&ang_cfg->angels[i]);
		}
	}
}

void ANG_passive(char* number) {
	for (uint8_t i = 1; i < ANG_MAX_NR_ANGELS; i++) {
		if (strncmp(ang_cfg->angels[i].phonenumber, number, ANG_NUMBERSIZE) == 0) {
			ang_cfg->angels[i].state = ANG_STATE_PASSIVE;
		}
	}
}

void ANG_active(char* number){
	for (uint8_t i = 1; i < ANG_MAX_NR_ANGELS; i++) {
		if (strncmp(ang_cfg->angels[i].phonenumber, number, ANG_NUMBERSIZE) == 0) {
			ang_cfg->angels[i].state = ANG_STATE_ACTIVE;
		}
	}
}

void ang_clearAngel(ang_AngelStruct* ang)
{
	ang->state = ANG_STATE_EMPTY;
	memset(&ang->phonenumber, 0, ANG_NUMBERSIZE + 1);
	memset(&ang->message, 0, ANG_MESSAGESIZE + 1);
}

void ANG_getInitialConfig(ANG_config* cfg)
{
	ANG_init(cfg);
}

