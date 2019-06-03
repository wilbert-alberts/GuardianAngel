/*
 * Motion.c
 *
 *  Created on: 22 apr. 2019
 *      Author: wilbert
 */

#include "Motion.h"
#include "Timer.h"

#include <stdlib.h>
#include "Arduino.h"


#define MOTION_DEADTIME (10)
#define MOTION_PIRPIN   (5)

typedef enum
{
	MTN_STATE_IDLE,
	MTN_STATE_FIRED
} mtn_detectorStateEnum ;

typedef struct
{
	ID_id     id;    // Set to ID_NULL when not used
	MTN_Func  func;
	void*     context;
} mtn_ListenerStruct;

typedef struct
{
	mtn_detectorStateEnum state;
	uint8_t                  deadTime;
} mtn_DetectorStruct;

static mtn_ListenerStruct mtn_listeners[MTN_MAX_LISTENERS];
static mtn_DetectorStruct mtn_detector;
static ID_id              mtn_nextId;

static void mtn_tick(void* context);
static void mtn_tickWhileIdle(uint8_t motionDetected);
static void mtn_tickWhileFired(uint8_t motionDetected);
static uint8_t mtn_detectMotion();
static void mtn_clearListener(mtn_ListenerStruct* listener);

void MTN_init()
{
	Serial.println(F("> MTN_init()"));
	// Initialize listener administration;
	for (int i=0; i< MTN_MAX_LISTENERS; i++) {
		mtn_clearListener(&mtn_listeners[i]);
	}
	ID_init(&mtn_nextId);

	// Setup IO (pin, pin direction etc).
	pinMode(MOTION_PIRPIN, INPUT);
	mtn_detector.state = MTN_STATE_IDLE;

	// Register motion_tick with 10ms period
	TMR_registerCB(mtn_tick, NULL, 10);
	Serial.println(F("< MTN_init()"));
}

ID_id MTN_addListener(MTN_Func listener, void* context)
{
	for (int i=0; i< MTN_MAX_LISTENERS; i++) {
		if (mtn_listeners[i].id == 0) {
			mtn_listeners[i].id = ID_getNext(&mtn_nextId);
			mtn_listeners[i].func = listener;
			mtn_listeners[i].context = context;
			return mtn_listeners[i].id;
		}
	}
	// Error: unable to add listener; no free slot;
	return 0;
}

void MTN_removeListener(ID_id id)
{
	for (int i=0; i< MTN_MAX_LISTENERS; i++) {
		if (mtn_listeners[i].id == id) {
			mtn_clearListener(&mtn_listeners[i]);
		}
	}
}

static void mtn_tick(void* context)
{
	uint8_t motion = mtn_detectMotion();
	switch (mtn_detector.state) {
	case MTN_STATE_IDLE:
		mtn_tickWhileIdle(motion);
		break;
	case MTN_STATE_FIRED:
		mtn_tickWhileFired(motion);
		break;
	default:
		// Error: not all states covered;
		break;
	}
}

static void mtn_tickWhileIdle(uint8_t motionDetected)
{
	if (motionDetected != 0) {
//		Serial.println(F("*"));
		for (int i=0; i<MTN_MAX_LISTENERS; i++) {
			if (mtn_listeners[i].id != 0) {
				mtn_listeners[i].func(mtn_listeners[i].context);
			}
		}
		mtn_detector.deadTime = MOTION_DEADTIME;
		mtn_detector.state = MTN_STATE_FIRED;
	}
}

static void mtn_tickWhileFired(uint8_t motionDetected)
{
	if (motionDetected == 0) {
		mtn_detector.deadTime--;
		if (mtn_detector.deadTime == 0) {
			mtn_detector.state = MTN_STATE_IDLE;
//			Serial.println('.');
		}
	}
	else {
		mtn_detector.deadTime = MOTION_DEADTIME;
	}

}

static uint8_t mtn_detectMotion()
{
	return digitalRead(MOTION_PIRPIN);
}

static void mtn_clearListener(mtn_ListenerStruct* listener)
{
	listener->id = ID_NULL;
	listener->func = NULL;
	listener->context = NULL;
}
