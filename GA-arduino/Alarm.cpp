/*
 * Alarm.cpp
 *
 *  Created on: 30 apr. 2019
 *      Author: wilbert
 */

#include "Angel.h"
#include "Log.h"


void ALM_init()
{
}

void ALM_raiseAlarm()
{
	LOG_entry("ALM_raiseAlarm");
	ANG_alarmAll();
	LOG_exit("ALM_raiseAlarm");
}


