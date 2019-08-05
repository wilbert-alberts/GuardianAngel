/*
 * GSM.h
 *
 *  Created on: May 13, 2019
 *      Author: ben
 */

#ifndef GSM_H_
#define GSM_H_

#include "Timestamp.h"
#include "ID.h"

#define GSM_NUMBERSIZE     (10)
#define GSM_MESSAGESIZE    (160)
#define GSM_MAX_SMS_SUBSCRIBERS (3)

typedef char* GSM_Number;
typedef char* GSM_Message;

typedef  void (*ReceiveCB)(GSM_Number number, GSM_Message message);

void GSM_init();
void GSM_getTime(TS_timestamp* ts, uint8_t* seconds);
void GSM_sendSMS(GSM_Number number, GSM_Message message);
ID_id GSM_registerReceiveCB(ReceiveCB cb);

#endif /* GSM_H_ */
