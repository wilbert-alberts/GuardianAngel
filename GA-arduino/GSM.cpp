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
#include "ID.h"

#define RX 3
#define TX 2
#define RESET 7
#define BAUD 9600

typedef struct {
	ID_id id;
	ReceiveCB cb;
} GSM_smsSubscriber;

static GSMSim gsm(RX, TX, RESET);
static void gsm_checkSMS(void *context);
static void gsm_receiveSMS(int nr);
static void gsm_clearSubscriber(GSM_smsSubscriber *sub);

static GSM_smsSubscriber gsm_smsSubscriber[GSM_MAX_SMS_SUBSCRIBERS];
static ID_id sdl_subscriberId;

void GSM_init() {
	LOG_entry("GSM_init");

	gsm.start();
	gsm.reset();
	gsm.smsTextMode(true);
	// Maybe send AT+CMEE=[2]

	for (uint8_t i = 1; i < GSM_MAX_SMS_SUBSCRIBERS; i++) {
		gsm_clearSubscriber(&gsm_smsSubscriber[i]);
	}

	if (gsm.pinStatus() == 1) {
		LOG("GSM_init(): pin required, sending 0000.");
		gsm.print("AT+CPIN=\"0000\"\r");
		LOG("GSM_init(): pin required, sent 0000.");
	}

	if (gsm.pinStatus() != 0) {
		LOG_noCR("GSM_init(): pin status not OK: ");
		LOG_nf(gsm.pinStatus());
		LOG_nf("\n");
	}

	// Clear all SMS's
	gsm.smsDeleteAll();
 	TMR_registerCB(gsm_checkSMS, NULL, 10000);

	LOG_exit("GSM_init");
}

void GSM_getTime(TS_timestamp *ts, uint8_t *seconds) {
	int day;
	int month;
	int year;
	int hour;
	int minute;
	int second;

	LOG_entry("GSM_getTime");
	gsm.timeGet(&day, &month, &year, &hour, &minute, &second);

	ts->day = day - 1;
	ts->month = month - 1;
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

void GSM_sendSMS(GSM_Number number, GSM_Message message) {
	LOG_entry("GSM_sendSMS");
	LOG_nf(F("= GSM_sendSMS(), sending to: "));
	LOG_nf(number);
	LOG_nf(F(", message: "));
	LOG_nf(message);
	LOG_nf(F("\n"));;
	gsm.smsSend(number, message);
	LOG_exit("GSM_sendSMS");
}

ID_id GSM_registerReceiveCB(ReceiveCB cb) {
	LOG_entry("GSM_registerReceiveCB");

	for (uint8_t i = 0; i < GSM_MAX_SMS_SUBSCRIBERS; i++) {
		if (gsm_smsSubscriber[i].id == ID_NULL) {
			gsm_smsSubscriber[i].id = ID_getNext(&sdl_subscriberId);
			gsm_smsSubscriber[i].cb = cb;
			LOG_nf(F("= GSM_registerReceiveCB(), returned cb id: "));
			LOG_nf(gsm_smsSubscriber[i].id);
			LOG_nf(F("\n"));
			LOG_exit("SDL_addTask")
			return gsm_smsSubscriber[i].id;
		}
	}
	LOG("GSM_registerReceiveCB(): unable to add subscriber, out of free slots.");
	LOG_exit("GSM_registerReceiveCB");
	return ID_NULL;
}

static void gsm_checkSMS(void *context) {
	LOG_entry("gsm_checkSMS");
	String messages = gsm.smsListUnread();
	Serial.println("msg: ===");
	Serial.println(messages);
	Serial.println("===");
	if (messages.startsWith("NOSMS")) {
		LOG_nf(F("= gsm_checkSMS(), no SMSs"));
		LOG_exit("gsm_checkSMS");
		return;
	}
	LOG_nf(F("= gsm_checkSMS(), SMS received"));

	if (messages.startsWith("SMSIndexNo:")) {
		LOG_nf(F("= gsm_checkSMS(), found new SMSs"));

		messages = messages.substring(strlen("SMSIndexNo:"));
		while (messages.indexOf(',') > 0) {
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

	}

	LOG_exit("gsm_checkSMS");
}

static void gsm_receiveSMS(int nr) {
	LOG_entry("gsm_receiveSMS");
	LOG("nr: ");
	LOG_nf(nr);
	String msg = gsm.smsRead(nr);
	LOG("msg: ")
	LOG_nf(msg);
	LOG("\n");
	for (uint8_t i=0; i<GSM_MAX_SMS_SUBSCRIBERS; i++) {
		if (gsm_smsSubscriber[i].id != ID_NULL) {
			gsm_smsSubscriber[i].cb(nr, msg.c_str());
		}
	}
	LOG_exit("gsm_receiveSMS");
}

static void gsm_clearSubscriber(GSM_smsSubscriber *sub) {
	sub->id = ID_NULL;
	sub->cb = NULL;
}
