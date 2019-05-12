/*
 * Config.cpp
 *
 *  Created on: 30 apr. 2019
 *      Author: wilbert
 */

#include "Config.h"

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

static char cfg_expectedMagic[8] = "WALB+BEN";

static void cfg_readAll();
static void cfg_writeAll();
static void cfg_persistInitialContent();

void CFG_init()
{
	cfg_readAll();
}

CFG_config* CFG_get()
{
	return &cfg_eConfig.config;
}

void CFG_persist()
{
	cfg_writeAll();
}

static void cfg_readAll()
{
	char* cfgPtr = (char*)&cfg_eConfig;

	for (uint16_t a=0; a<8; a++) {
		char ch = EEPROM.read(a);
		cfgPtr[a] = ch;
		cfgPtr++;
		if (ch != cfg_expectedMagic[a]) {
			cfg_persistInitialContent();
			return;
		}
	}
	for (uint16_t a=8; a< sizeof(cfg_eConfig); a++) {
		char ch = EEPROM.read(a);
		cfgPtr[a] = ch;
		cfgPtr++;
	}
}

static void cfg_persistInitialContent()
{
	for (uint16_t a=0; a<8; a++) {
		cfg_eConfig.magicNr[a] = cfg_expectedMagic[a];
	}
	cfg_eConfig.versionNr = CFG_VERSION;
	cfg_eConfig.nrPersist = 0;
	ANG_getInitialConfig(&(cfg_eConfig.config.ang_config));
	GRD_getInitialConfig(&(cfg_eConfig.config.grd_config));

	cfg_writeAll();
}

static void cfg_writeAll()
{
	char* cfgPtr = (char*)&cfg_eConfig;
	cfg_eConfig.nrPersist++;

	for (uint16_t a=0; a< sizeof(cfg_eConfig); a++) {
		EEPROM.write(a, cfgPtr[a]);
	}
}
