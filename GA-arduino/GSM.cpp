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
#include "Timer.h"
#include "GSM.h"

#define RX 7
#define TX 8
#define RESET 2
#define BAUD 9600

static GSMSim gsm(RX, TX, RESET);
static void gsm_checkSMS(void* context);
static void gsm_receiveSMS(int nr);

void GSM_init()
{
	LOG_entry("GSM_init");
  /*
	gsm.start();

	if (gsm.pinStatus() == 1) {
		LOG("GSM_init(): pin required, sending 0000.");
		gsm.print("AT+CPIN=\"0000\"");
		LOG("GSM_init(): pin required, sent 0000.");
	}

	if (gsm.pinStatus() != 0) {
		LOG_noCR("GSM_init(): pin status not OK: ");
		LOG_nf(gsm.pinStatus());
    LOG_nf("\n");
	}

	// Clear all SMS's
	gsm.smsDeleteAllRead();
 */
	TMR_registerCB(gsm_checkSMS, NULL, 1000);

	LOG_exit("GSM_init");
}

void GSM_getTime(TS_timestamp* ts, uint8_t* seconds)
{
	int day;
	int month;
	int year;
	int hour;
	int minute;
	int second;

	LOG_entry("GSM_getTime");
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

	LOG_exit("GSM_getTime");
}

void GSM_sendSMS(GSM_Number number, GSM_Message message)
{
	LOG_entry("GSM_sendSMS");
	LOG_nf(F("= GSM_sendSMS(), sending to: "));
	LOG_nf(number);
	LOG_nf(F(", message: "));
	LOG_nf(message);
	LOG_nf(F("\n"));;
	gsm.smsSend(number, message);
	LOG_exit("GSM_sendSMS");
}

static void GSM_registerReceiveCB(ReceiveCB cb)
{

}

static void gsm_checkSMS(void* context)
{
	LOG_entry("gsm_checkSMS");
	String messages = gsm.smsListUnread();
	if (messages.startsWith("NOSMS")) {
		LOG_nf(F("= gsm_checkSMS(), no SMSs"));
		return;
	}

	if (messages.startsWith("SMSIndexNo:")) {
		LOG_nf(F("= gsm_checkSMS(), found new SMSs"));

		messages = messages.substring(strlen("SMSIndexNo:"));
		while( messages.indexOf(',') > 0) {
			String nrStr = messages.substring(0, messages.indexOf(','));
			messages = messages.substring(messages.indexOf(','));
			nrStr.trim();
			int nr = nrStr.toInt();
			gsm_receiveSMS(nr);
		}
		// Handle final number
		String nrStr = messages;
		nrStr.trim();
		int nr = nrStr.toInt();
		gsm_receiveSMS(nr);

		return;
	}

}

static void gsm_receiveSMS(int nr)
{
	String msg = gsm.smsRead(nr);

}
