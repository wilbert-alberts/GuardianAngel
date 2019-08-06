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
static bool cfg_cfgRead = false;


static char cfg_expectedMagic[9] = "WALB+BEN";

static void cfg_readAll();

void CFG_init()
{
	LOG_entry("CFG_init");
	cfg_readAll();
	LOG_exit("CFG_init");
}

void CFG_clear()
{
  LOG_entry("CFG_clear");
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }  
  LOG_exit("CFG_clear");
}

bool CFG_getCfgAng(ANG_config** ang_config)
{
	*ang_config = &cfg_eConfig.config.ang_config;
	return cfg_cfgRead;
}

bool CFG_getCfgGrd(GRD_config** grd_config)
{
	*grd_config = &cfg_eConfig.config.grd_config;
	return cfg_cfgRead;
}

void CFG_persist()
{
	LOG_entry("CFG_persist");
	char* cfgPtr = (char*)&cfg_eConfig;
	cfg_eConfig.nrPersist++;

	LOG("cfg_writeAll(): writing EEPROM.");
	EEPROM.put(0, cfg_eConfig);
	LOG("cfg_writeAll(): writing EEPROM done.");

	LOG_exit("CFG_persist");
}

static void cfg_readAll()
{
	char* cfgPtr = (char*)&cfg_eConfig;

	LOG_entry("cfg_readAll");

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
			cfg_cfgRead = false;
			LOG_exit("cfg_readAll");
			return;
		}
	}
	LOG("cfg_readAll(): config succesfully read.");
	cfg_cfgRead = true;
	LOG_exit("cfg_readAll");
}
