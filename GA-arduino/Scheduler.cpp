/*
 * Scheduler.h
 *
 *  Created on: 22 apr. 2019
 *      Author: wilbert
 */

#include <Arduino.h>
#include <stdlib.h>

#include "Timestamp.h"
#include "Scheduler.h"
#include "Timer.h"
#include "Log.h"

typedef struct {
	ID_id id;
	TS_timestamp when;
	SDL_taskFunc func;
	void* context;
} SDL_taskStruct;

static SDL_taskStruct sdl_tasks[SDL_MAX_NR_TASKS];
static ID_id sdl_taskId;
static TS_timestamp sdl_now;
static uint8_t sdl_seconds;
static bool sdl_ticking;

static void sdl_initTime();
static void sdl_tick(void* context);
static void sdl_clearTask(SDL_taskStruct* task);
static void sdl_handleTasks();

void SDL_init() {
	LOG_entry("SDL_init()");
	for (uint8_t i = 1; i < SDL_MAX_NR_TASKS; i++) {
		sdl_clearTask(&sdl_tasks[i]);
	}

	sdl_initTime();
	TMR_registerCB(sdl_tick, NULL, 1000);
	sdl_ticking = true;
	LOG_exit("SDL_init()");
}

ID_id SDL_addTask(const TS_timestamp* moment, SDL_taskFunc task,
		void* context) {
	for (uint8_t i = 0; i < SDL_MAX_NR_TASKS; i++) {
		if (sdl_tasks[i].id == ID_NULL) {
			sdl_tasks[i].id = ID_getNext(&sdl_taskId);
			sdl_tasks[i].func = task;
			sdl_tasks[i].context = context;
			sdl_tasks[i].when = *moment;
			return sdl_tasks[i].id;
		}
	}
	LOG("SDL_addTask(): unable to add task, out of free slots.");
	return ID_NULL;
}

void SDL_getTime(TS_timestamp* dest) {
	*dest = sdl_now;
}

void SDL_removeTask(ID_id task) {
	for (uint8_t i = 0; i < SDL_MAX_NR_TASKS; i++) {
		if (sdl_tasks[i].id == task) {
			sdl_clearTask(&sdl_tasks[i]);
		}
	}
}

uint8_t SDL_setTime(const TS_timestamp* now, uint8_t secs) {
	LOG_entry("SDL_setTime");

	if (TS_compare(&sdl_now, now) < 0) {
		LOG("SDL_setTime(): SDL is minutes behind, catch up.");
		sdl_now = *now;
		sdl_seconds = secs;
		sdl_ticking = true;
	} else {

		if ((TS_compare(&sdl_now, now) == 0) && (secs > sdl_seconds)) {
			LOG("SDL_setTime(): SDL is seconds behind, catch up.");
			sdl_seconds = secs;
			sdl_ticking = true;
		} else {

			LOG("SDL_setTime(): SDL is ahead, stop ticking");
			sdl_ticking = false;
		}
	}

	LOG_exit("SDL_setTime");
	return -1;
}

static void sdl_tick(void* context) {
	if (sdl_ticking) {
		sdl_seconds++;
		if (sdl_seconds == 60) {
			sdl_seconds = 0;
			TS_addMinutes(&sdl_now, 1);
			TS_print(&sdl_now);
			Serial.println(F(""));
			sdl_handleTasks();
		}
	}
}

static void sdl_initTime() {
	sdl_now.year = 2000;
	sdl_now.month = 0;
	sdl_now.day = 0;
	sdl_now.hour = 0;
	sdl_now.minute = 0;
	sdl_seconds = 0;
}

static void sdl_handleTasks() {
	for (uint8_t i = 0; i < SDL_MAX_NR_TASKS; i++) {
		if (sdl_tasks[i].id != ID_NULL) {
			if (TS_compare(&sdl_now, &sdl_tasks[i].when) >= 0) {
				SDL_taskFunc func = sdl_tasks[i].func;
				void* context = sdl_tasks[i].context;
				sdl_clearTask(&sdl_tasks[i]);
				func(context);
			}
		}
	}
}

static void sdl_clearTask(SDL_taskStruct* task) {
	task->id = ID_NULL;
	task->func = NULL;
	task->context = NULL;
	task->when = TS_never;
}
