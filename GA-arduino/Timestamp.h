/*
 * Timestamp.h
 *
 *  Created on: 22 apr. 2019
 *      Author: wilbert
 */

#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

#include <stdint.h>

typedef struct TS_timestamp
{
	uint16_t year;
	uint8_t  month;  // Note: january is 0
	uint8_t  day;    // Note: day starts with 0 at the 1st of every month
	uint8_t  hour;   // Note: 0-23
	uint8_t  minute; // Note: --59
} TS_timestamp;

typedef struct TS_HHmm
{
	uint8_t  hour;
	uint8_t  minute;
} TS_HHmm;

extern const TS_timestamp TS_never;


void TS_create(TS_timestamp* ts, uint16_t year, uint8_t month,  uint8_t day,  uint8_t hour,  uint8_t minute);
void TS_add(TS_timestamp* ts, uint16_t year, uint16_t month,  uint16_t day,  uint16_t hour,  uint16_t minute);
void TS_addMinutes(TS_timestamp* ts, uint16_t minutes);
void TS_addHours(TS_timestamp* ts, uint16_t hours);
void TS_addDays(TS_timestamp* ts, uint16_t days);
void TS_addMonths(TS_timestamp* ts, uint16_t months);
void TS_addYears(TS_timestamp* ts, uint16_t years);
int8_t TS_compare(const TS_timestamp* left, const TS_timestamp* right); // Determine left - right

void TS_print(const TS_timestamp* p);

#endif /* TIMESTAMP_H_ */
