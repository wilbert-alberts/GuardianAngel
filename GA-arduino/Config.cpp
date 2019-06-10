/*
 * Config.cpp
 *
 *  Created on: 30 apr. 2019
 *      Author: wilbert
 */

#include <Arduino.h>
#include "Config.h"
#include "Log.h"

#include <stdint.h>
#include <EEPROM.h>

typedef struct
{
	char magicNr[8];
	uint8_t versionNr;
	uint8_t nrPersist;

	CFG_config config;
} cfg_EConfigStruct;

static cfg_EConfigStruct cfg_eConfig;

static char cfg_expectedMagic[9] = "WALB+BEN";

static void cfg_readAll();
static void cfg_writeAll();
static void cfg_persistInitialContent();

void CFG_init()
{
	LOG_entry("CFG_init()");
	cfg_readAll();
	LOG_exit("CFG_init()");
}

CFG_config* CFG_get()
{
	return &cfg_eConfig.config;
}

void CFG_persist()
{
	LOG_entry("CFG_persist()");
	cfg_writeAll();
	LOG_exit("CFG_persist()");
}

static void cfg_readAll()
{
	char* cfgPtr = (char*)&cfg_eConfig;

	LOG_entry("cfg_readAll()");

	LOG("cfg_readAll(): Reading EEPROM content.");
	EEPROM.get(0, cfg_eConfig);
	LOG("cfg_readAll(): Reading EEPROM content ready.");

	for (uint16_t a=0; a<8; a++) {
		char ch = cfgPtr[a];
//		Serial.print(a);
//		Serial.print(": ");
//		Serial.println(ch);

		if (ch != cfg_expectedMagic[a]) {
			LOG("cfg_readAll(): Invalid header.");
			cfg_persistInitialContent();
			LOG_exit("cfg_readAll()");
			return;
		}
	}
	LOG_exit("cfg_readAll()");
}

static void cfg_persistInitialContent()
{
	LOG_entry("cfg_persistInitialContent()");
	for (uint16_t a=0; a<8; a++) {
		cfg_eConfig.magicNr[a] = cfg_expectedMagic[a];
	}
	cfg_eConfig.versionNr = CFG_VERSION;
	cfg_eConfig.nrPersist = 0;
	ANG_getInitialConfig(&(cfg_eConfig.config.ang_config));
	GRD_getInitialConfig(&(cfg_eConfig.config.grd_config));

	cfg_writeAll();
	LOG_exit("cfg_persistInitialContent()");
}

static void cfg_writeAll()
{
	LOG_entry("cfg_writeAll()");

	char* cfgPtr = (char*)&cfg_eConfig;
	cfg_eConfig.nrPersist++;

	LOG("cfg_writeAll(): writing EEPROM.");
	EEPROM.put(0, cfg_eConfig);
	LOG("cfg_writeAll(): writing EEPROM done.");
	LOG_exit("cfg_writeAll()");
}
