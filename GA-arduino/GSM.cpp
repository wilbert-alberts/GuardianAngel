/*
 * GSM.cpp
 *
 *  Created on: May 13, 2019
 *      Author: ben
 */


#include <Arduino.h>
#include <avr/pgmspace.h>
#include <GSMSim.h>

#include "Log.h"
#include "Timestamp.h"
#include "GSM.h"

#define RX 7
#define TX 8
#define RESET 2
#define BAUD 9600

static GSMSim gsm(RX, TX, RESET);

void GSM_init()
{
	LOG_entry("GSM_init()");
	gsm.start();

	if (gsm.pinStatus() == 1) {
		LOG("GSM_init(): pin required, sending 0000.");
		gsm.print("AT+CPIN=\"0000\"");
		LOG("GSM_init(): pin required, sent 0000.");
	}

	if (gsm.pinStatus() != 0) {
		LOG("GSM_init(): pin status not OK.");
		LOG_nf(gsm.pinStatus());
	}

	LOG_exit("GSM_init()");
}

void GSM_getTime(TS_timestamp* ts, uint8_t* seconds)
{
	int day;
	int month;
	int year;
	int hour;
	int minute;
	int second;

	LOG_entry("GSM_getTime()");
	gsm.timeGet(&day, &month, &year, &hour, &minute, &second);

	ts->day = day-1;
	ts->month = month-1;
	ts->year = year;
	ts->hour = hour;
	ts->minute = minute;
	*seconds = second;

	LOG("GSM_getTime(): time: ");
	TS_print(ts);
	LOG_nf(F(":"));
	LOG_nf(*seconds);
	LOG_nf(F(".\n"));

	LOG_exit("GSM_getTime()");
}

void GSM_sendSMS(GSM_Number number, GSM_Message message)
{

}

void GSM_registerReceiveCB(ReceiveCB cb)
{

}



