/*
 * Config.h
 *
 *  Created on: 30 apr. 2019
 *      Author: wilbert
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "Angel.h"
#include "GuardTask.h"

#define CFG_VERSION (1)

typedef struct
{
	ANG_config ang_config;
	GRD_config grd_config;

} CFG_config;


void CFG_init();
CFG_config* CFG_get();
void CFG_persist();

#endif /* CONFIG_H_ */
