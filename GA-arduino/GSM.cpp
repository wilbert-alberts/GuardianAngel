/*
 * GSM.cpp
 *
 *  Created on: May 13, 2019
 *      Author: ben
 */


#include <Arduino.h>
#include <avr/pgmspace.h>
#include <GSMSim.h>


#include "Timestamp.h"
#include "GSM.h"

#define RX 7
#define TX 8
#define RESET 2
#define BAUD 9600

static GSMSim gsm(RX, TX, RESET);

void GSM_init()
{
	Serial.println(F("> GSM_init()"));
	gsm.start();

	if (gsm.pinStatus() == 1) {
		Serial.println(F("= GSM_init(): pin required, sending 0000"));
		gsm.print("AT+CPIN=\"0000\"");
		Serial.println(F("= GSM_init(): pin required, sent 0000"));
	}

	if (gsm.pinStatus() != 0) {
		Serial.print(F("= GSM_init(): pin status not ok:"));
		Serial.print(gsm.pinStatus());
	}

	Serial.println(F("< GSM_init()"));
}

void GSM_getTime(TS_timestamp* ts, uint8_t* seconds)
{
	int day;
	int month;
	int year;
	int hour;
	int minute;
	int second;

	Serial.println(F("> GSM_getTime()"));
	gsm.timeGet(&day, &month, &year, &hour, &minute, &second);

	ts->day = day-1;
	ts->month = month-1;
	ts->year = year;
	ts->hour = hour;
	ts->minute = minute;
	*seconds = second;

	Serial.println(F("= GSM_getTime(): time: "));
	TS_print(ts);
	Serial.print(":");
	Serial.print(*seconds);
	Serial.println(F("."));

	Serial.println(F("< GSM_getTime()"));
}

void GSM_sendSMS(GSM_Number number, GSM_Message message)
{

}

void GSM_registerReceiveCB(ReceiveCB cb)
{

}



