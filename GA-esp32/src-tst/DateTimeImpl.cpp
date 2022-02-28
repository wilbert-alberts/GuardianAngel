/*
 * DateTimeImpl.cpp
 *
 *  Created on: 4 dec. 2021
 *      Author:      ben
 *      Stolen from: wilbert
 */

#include <stdexcept>
#include <memory>
#include <string.h>
#include <stdio.h>
#include <sstream>
//#include <Arduino.h>  // for Serial.println

#include "DateTime.hpp"

class DateTimeImpl: public DateTime {
public:
	DateTimeImpl(int yy, int mm, int dd, int h, int m, int s);
	virtual ~DateTimeImpl();

	virtual int getYear() const;
	virtual int getMonth() const;
	virtual int getDay() const;
	virtual int getHours() const;
	virtual int getMinutes() const;
	virtual int getSeconds() const;

	virtual void add(int h, int m, int s) {
		addSeconds((h * 60 + m) * 60 + s);
	}
	virtual void add(const DateTime &other);
	virtual void addDays(int dd);
	virtual void addHours(int h);
	virtual void addMinutes(int m);
	virtual void addSeconds(int s);
	virtual void addDST(int dstValue);

	virtual int compareTo(const DateTime &other) const;

	virtual std::string toString(bool doDate = true, bool doTime = true) const;

private:
	int year;
	int month;
	int day;
	int hour;
	int min;
	int sec;

	void verifyIntegrity();
	void verifyIntegrity(int yy, int mm, int dd, int h, int m, int s);
	void verifyNatural(int value);
	void verifyNaturalMin(int value, int bound);
	void verifyNaturalMax(int value, int bound);
};

static const int monthLengths[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31,
		30, 31 };
static const char *monthNames[13] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec", "ILLEGAL" };

//------------
static int ciStrcmp(const char *s1, const char *s2)
// case insensitive strcmp
		{
	char s1c[100];
	char s2c[100];
	strncpy(s1c, s1, 100);
	strncpy(s2c, s2, 100);
	int sl = strlen(s1c);
	for (int i = 0; i < sl; i++) {
		s1c[i] = (char) tolower(s1c[i]);
		s2c[i] = (char) tolower(s2c[i]);
	}
	return (strcmp(s1c, s2c));
}

//---------------
static int monthNameToInt(char *name)
// match a 3-letter month name, case-insensitive
// Returns: month number (1..12) or -1 if no match
		{
	int i;
	for (i = 0; i < 12; i++) {
		if (ciStrcmp(name, monthNames[i]) == 0) {
			break;
		}
	}
	if (i == 12) { // no match
		i = -1;
	} else
		i += 1;
	return i;
}

//-------------------
static const char* monthName(int i) {
	if (i < 1 || i > 13)
		i = 13;
	return monthNames[i - 1];
}

//-------------------
static int monthLength(int month, int year) {
	if ((month < 1) || (month > 12))
		throw new std::range_error("illegal month number");
	int ml = monthLengths[month - 1];
	if ((year % 4 == 0) && (year % 100 != 0))
		ml += 1;
	return ml;
}

//---------------------------
void DateTimeImpl::addSeconds(int seconds)
// add the seconds and handle the carries
// note: seconds can be negative 
		{
	sec += seconds;
	if (sec >= 0) {
		min += sec / 60;
		sec = sec % 60;
	} else { // % not defined for neg args
		while (sec < 0) {
			sec += 60;
			min -= 1;
		}
	}
	if (min >= 0) {
		hour += min / 60;
		min = min % 60;
	} else {
		while (min < 0) {
			hour -= 1;
			min += 60;
		}
	}
	// did we have carries into the hour?
	if (hour >= 0) {
		day += hour / 24;
		hour = hour % 24;
	} else {
		while (hour < 0) {
			hour += 24;
			day -= 1;
		}
	}
	// did we have carries into the day?
	while (day <= 0) {
		month -= 1;
		day += monthLength(month, year);
	}
	while (day > monthLength(month, year)) {
		day -= monthLength(month, year);
		month++;
	}
	while (month > 12) {
		month -= 12;
		year += 1;
	}
	while (month <= 0) {
		month += 12;
		year -= 1;
	}
}

namespace DateTimeFactory {
std::shared_ptr<DateTime> create(int yy, int mm, int dd, int h, int m, int s) {
	return std::shared_ptr<DateTime>(new DateTimeImpl(yy, mm, dd, h, m, s));
}

//----------------------------
std::shared_ptr<DateTime> create(std::string s) {
	try {
		const char *theString = s.c_str();
		char monthName[10];
		int tz = 0;
		int unknown = 0;

		int year = 0;
		int month = 1;
		int day = 1;
		int hour = 0;
		int min = 0;
		int sec = 0;

		//                               2021,12,24,13,10,45,"+4",0
		int nMatches = sscanf(theString, "%d,%d,%d,%d,%d,%d,\"%d\",%d", &year,
				&month, &day, &hour, &min, &sec, &tz, &unknown);
		if (nMatches != 8) {
			//                            21/12/31,22:38:16
			nMatches = sscanf(theString, "%d/%d/%d,%d:%d:%d%d", &year, &month,
					&day, &hour, &min, &sec, &tz);
		}
		if (nMatches != 7) {
			//                            23 mar 2021 15:12:59
			nMatches = sscanf(theString, "%d %3s %d %d:%d:%d%d", &day,
					monthName, &year, &hour, &min, &sec, &tz);
		}
		if (nMatches >= 3) { // we have at least the date
			month = monthNameToInt(monthName);

			if (nMatches == 3) {  // 23 jan 2021: date only;
			}
		}
		if (nMatches < 3) {  // no date, check for time
			//                            12:21:15+4
			nMatches = sscanf(theString, " %d:%d:%d%d", &hour, &min, &sec, &tz);
		}
		if (nMatches < 2) {  // at least hour and min needed
			throw new std::range_error(
					"DateTime from string: string does not match");
		}
		if (year < 100)
			year += 2000;

		if (month > 0 && month <= 12 && day > 0 && day <= 32 && hour >= 0
				&& hour < 24 && min >= 0 && min < 60 && sec >= 0 && sec < 60) {
			std::shared_ptr<DateTime> dt = create(year, month, day, hour, min,
					sec);
			dt->addSeconds(tz * 15 * 60);
			return dt;
		} else
			return std::shared_ptr<DateTime>(nullptr);

	} catch (...) {
		return std::shared_ptr<DateTime>(nullptr);
	}
}
} // namespace

//------------------
DateTimeImpl::DateTimeImpl(int yy, int mm, int dd, int h, int m, int s) {
	year = yy;
	month = mm;
	day = dd;
	hour = h;
	min = m;
	sec = s;
	verifyIntegrity();
}

DateTimeImpl::~DateTimeImpl() {
}

int DateTimeImpl::getYear() const {
	return year;
}
int DateTimeImpl::getMonth() const {
	return month;
}
int DateTimeImpl::getDay() const {
	return day;
}
int DateTimeImpl::getHours() const {
	return hour;
}
int DateTimeImpl::getMinutes() const {
	return min;
}
int DateTimeImpl::getSeconds() const {
	return sec;
}

// NOTE: DOES NOT ADD DAYS!
void DateTimeImpl::add(const DateTime &other) {
	add(other.getHours(), other.getMinutes(), other.getSeconds());
}

void DateTimeImpl::addDays(int dd) {
	addHours(dd * 24);
}

void DateTimeImpl::addHours(int h) {
	addMinutes(h * 60);
}

void DateTimeImpl::addMinutes(int m) {
	addSeconds(m * 60);
}

void DateTimeImpl::addDST(int dstValue) {
	if (dstValue < 0 || dstValue > 2)
		dstValue = 0;
	addHours(dstValue);
}

int DateTimeImpl::compareTo(const DateTime &other) const {
	if (year < other.getYear())
		return -1;
	if (year > other.getYear())
		return 1;

	if (month < other.getMonth())
		return -1;
	if (month > other.getMonth())
		return 1;

	if (day < other.getDay())
		return -1;
	if (day > other.getDay())
		return 1;

	if (hour < other.getHours())
		return -1;
	if (hour > other.getHours())
		return 1;

	if (min < other.getMinutes())
		return -1;
	if (min > other.getMinutes())
		return 1;

	if (sec < other.getSeconds())
		return -1;
	if (sec > other.getSeconds())
		return 1;

	return 0;
}

void DateTimeImpl::verifyIntegrity() {
	verifyIntegrity(year, month, day, hour, min, sec);
}

void DateTimeImpl::verifyIntegrity(int yy, int mm, int dd, int h, int m,
		int s) {
	verifyNaturalMin(yy, 2000);
	verifyNaturalMax(yy, 2100);
	verifyNaturalMin(mm, 1);
	verifyNaturalMax(mm, 12);
	verifyNaturalMin(dd, 1);
	verifyNaturalMax(dd, monthLength(mm, yy));
	verifyNaturalMax(h, 24);
	verifyNaturalMax(m, 60);
	verifyNaturalMax(s, 60);
}

void DateTimeImpl::verifyNaturalMax(int value, int bound) {
	verifyNatural(value);
	if (value >= bound)
		throw new std::range_error("Too high");
}

void DateTimeImpl::verifyNaturalMin(int value, int bound) {
	if (value < bound)
		throw new std::range_error("Too low");
}

void DateTimeImpl::verifyNatural(int value) {
	if (value < 0)
		throw new std::range_error("Too low");
}

std::string DateTimeImpl::toString(bool doDate, bool doTime) const {
//   Serial.println ("Enter toString..." );
	std::stringstream ss;
	if (doDate) {
		ss << day;
		ss << ' ';
		ss << monthName(month);
		ss << ' ';
		ss << year;
	}
	if (doTime) {
		if (doDate)
			ss << "  ";
		if (hour < 10)
			ss << " ";
		ss << hour;
		ss << ':';
		ss << min;
		ss << ':';
		ss << sec;
	}
	return ss.str();
}
