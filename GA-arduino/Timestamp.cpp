/*
 * Timestamp.h
 *
 *  Created on: 22 apr. 2019
 *      Author: wilbert
 */

#include <Arduino.h>
#include "Timestamp.h"
#include "Log.h"

const TS_timestamp TS_never = { 0,0,0,0,0 };
const uint8_t ts_nominalDaysInMonth[12] = { 31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };


static uint8_t ts_daysInMonth(const TS_timestamp* ts);

void TS_create(TS_timestamp *ts, uint16_t year, uint8_t month,  uint8_t day,  uint8_t hour,  uint8_t minute)
{
	ts->year = year;
	ts->month = month;
	ts->day = day;
	ts->hour = hour;
	ts->minute = minute;
}

void TS_add(TS_timestamp *ts, uint16_t year, uint16_t month,  uint16_t day,  uint16_t hour,  uint16_t minute)
{
	if (minute>0) TS_addMinutes(ts, minute);
	if (hour>0)   TS_addHours(ts, hour);
	if (day>0)    TS_addDays(ts, day);
	if (month>0)  TS_addMonths(ts, month);
	if (year>0)   TS_addYears(ts, year);
}

void TS_addMinutes(TS_timestamp *ts, uint16_t minutes)
{
	uint16_t m = minutes + ts->minute;

	ts->minute = m % 60;
	if (m/60>0) TS_addHours(ts, m / 60);
}

void TS_addHours(TS_timestamp *ts, uint16_t hours)
{
	uint16_t h = hours+ ts->hour;

	ts->hour = h % 24;
	if (h/24>0) TS_addDays(ts, h / 24);
}

void TS_addDays(TS_timestamp *ts, uint16_t days)
{
	uint16_t d = ts->day + days;

	while (d > ts_daysInMonth(ts)-1) {
		d -= ts_daysInMonth(ts);
		TS_addMonths(ts, 1);
	}
	ts->day = d;
}

void TS_addMonths(TS_timestamp *ts, uint16_t months)
{
	uint16_t m = ts->month + months;

	ts->month = m % 12;
	if (m/12>0) TS_addYears(ts, m / 12);
}

void TS_addYears(TS_timestamp *ts, uint16_t years)
{
	ts->year += years;
}

int8_t TS_compare(const TS_timestamp* left, const TS_timestamp* right)
{
	if (left->year != right->year) {
		return left->year > right->year ? 1 : -1;
	}
	if (left->month!= right->month) {
		return left->month> right->month? 1 : -1;
	}
	if (left->day!= right->day) {
		return left->day> right->day? 1 : -1;
	}
	if (left->hour!= right->hour) {
		return left->hour> right->hour? 1 : -1;
	}
	if (left->minute!= right->minute) {
		return left->minute> right->minute? 1 : -1;
	}
	return 0;
}

uint8_t ts_daysInMonth(const TS_timestamp* ts)
{
	if (ts->month != 1) {
		return ts_nominalDaysInMonth[ts->month];
	}
	else {
		// If year not divisible by 4, then no leap year
		if (ts->year % 4 != 0) {
			return 28;
		}

		// If year divisible by 4, and by 100, then no leap year
		if ((ts->year % 4 == 0) && (ts->year % 100 == 0)) {
			return 28;
		}
		else {
			// Divisible by 4 but not by 100, leap year.
			return 29;
		}
	}
}

void TS_print(const TS_timestamp* p)
{
	LOG_nf(p->year);
	LOG_nf(F("-"));
	LOG_nf(p->month+1);
	LOG_nf(F("-"));
	LOG_nf(p->day+1);

	LOG_nf(F(" "));
	LOG_nf(p->hour);
	LOG_nf(F("-"));
	LOG_nf(p->minute);

}
