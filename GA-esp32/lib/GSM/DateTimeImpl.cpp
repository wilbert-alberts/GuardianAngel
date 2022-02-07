/*
 * DateTimeImpl.cpp
 *
 *  Created on: 4 dec. 2021
 *      Author:      ben
 *      Stolen from: wilbert
 */

#include <memory>
#include <string.h>
#include <stdio.h>
#include <sstream>

#include "DateTime.hpp"

static const int monthLengths[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static const char *monthNames[13] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec", "ILLEGAL"};

//------------
static int ciStrcmp(const char *s1, const char *s2)
// case insensitive strcmp
{
   char s1c[100];
   char s2c[100];
   strncpy(s1c, s1, 100);
   strncpy(s2c, s2, 100);
   int sl = strlen(s1c);
   for (int i = 0; i < sl; i++)
   {
      s1c[i] = (char)tolower(s1c[i]);
      s2c[i] = (char)tolower(s2c[i]);
   }
   return (strcmp(s1c, s2c));
}

//---------------
static int monthNameToInt(char *name)
// match a 3-letter month name, case-insensitive
// Returns: month number (1..12) or -1 if no match
{
   int i;
   for (i = 0; i < 12; i++)
   {
      if (ciStrcmp(name, monthNames[i]) == 0)
      {
         break;
      }
   }
   if (i == 12)
   { // no match
      i = -1;
   }
   else
      i += 1;
   return i;
}

//-------------------
static const char *monthName(int i)
{
   if (i < 1 || i > 13)
      i = 13;
   return monthNames[i - 1];
}

bool isLeap(int year)
{
   return ((year % 4 == 0) && (year % 100 != 0));
}

//-------------------
static int daysInMonth(int month, int year)
{
   if ((month < 1) || (month > 12))
      throw new std::range_error("illegal month number");
   int ml = monthLengths[month - 1];
   if ((month == 2) && (isLeap(year)))
      ml = 29;
   return ml;
}

//-------------------
static int daysInYear(int year)
{
   return isLeap(year) ? 366 : 365;
}

#ifdef NOTUSED
//-----------------
static int tzToSeconds(int tz)
{
   return tz * 15 * 60;
}
#endif


//--------------------------------------
static bool stringToComps(std::string s, int &year, int &month, int &day, int &hour, int &min, int &sec, int &tz)
// split a string into date/time components
{
   const char *theString = s.c_str();
   char monthName[10];

   year = 0;
   month = 1;
   day = 1;
   hour = 0;
   min = 0;
   sec = 0;
   tz = 0;

   // try to match several formats
   bool success = false;

   //                               2021,12,24,13,10,45,"+4"
   int nMatches = sscanf(theString, "%d,%d,%d,%d,%d,%d,\"%d\"",
                         &year, &month, &day, &hour, &min, &sec, &tz);
   if (nMatches == 7)
   {
      success = true;
   }

   if (!success)
   {
      //                            21/12/31,22:38:16+04
      nMatches = sscanf(theString, "%d/%d/%d,%d:%d:%d%d",
                        &year, &month, &day, &hour, &min, &sec, &tz);
      if ((nMatches == 7) || (nMatches == 6))
      { // with or without timezone
         success = true;
      }
   }
   if (!success)
   {
      //                            23 mar 2021 15:12:59+4
      nMatches = sscanf(theString, "%d %3s %d %d:%d:%d%d",
                        &day, monthName, &year, &hour, &min, &sec, &tz);
      if (nMatches >= 3)
      { // we have at least the date
         month = monthNameToInt(monthName);
         if (month > 0)
            success = true;
      }
   }

   if (!success)
   {
      // no date, check for time
      year = 0;
      month = 1;
      day = 1;
      hour = 0;
      min = 0;
      sec = 0;
      tz = 0;
      //                              12:21:15+4
      nMatches = sscanf(theString, " %d:%d:%d%d",
                        &hour, &min, &sec, &tz);
      if (nMatches >= 2)
      { // at least hour and min needed
         success = true;
      }
      else
      { // clean out result
         hour = 0;
      }
   }
   if (success)
   {

      if (year < 100)
         year += 2000;
      if (month > 0 && month <= 12 &&
          day > 0 && day <= 32 &&
          hour >= 0 && hour < 24 &&
          min >= 0 && min < 60 &&
          sec >= 0 && sec < 60)
      {
      }
      else
      {
         success = false;
      }
   }

   if (!success)
   {
      year = 2000;
      month = 1;
      day = 1;
      hour = 0;
      min = 0;
      sec = 0;
      tz = 0;
   }

   return success;
}

static void checkRange(int value, int min, int max)
{
   if ((value < min) || (value > max))
      throw new std::range_error("Value out of range");
}

static void verifyIntegrity(int yy, int mm, int dd, int h, int m, int s, int tz)
{
   checkRange(yy, 2000, 2200);
   checkRange(mm, 1, 12);
   checkRange(dd, 1, daysInMonth(mm, yy));
   checkRange(h, 0, 23);
   checkRange(m, 0, 59);
   checkRange(s, 0, 59);
   checkRange(tz, -8, 8);
}



//------------------
static int YMDtoDays(int yy, int mm, int dd)
{
   int days = 0;
   int rest = yy;
   while (rest > 2000)
   {
      days += daysInYear(--rest);
   }
   for (int i = 1; i < mm; i++)
      days += daysInMonth(i, yy);
   days += dd - 1;
   return days;
}

//------------------
static int HMStoSeconds(int h, int m, int s, int tz) {
// turns out that date/time already incorporates the timezone
   return  (h * 60 + m) * 60 + s;     // + tzToSeconds(tz);
}

//------------------
DateTime::DateTime(int yy, int mm, int dd, int h, int m, int s, int tz)
{
   verifyIntegrity(yy, mm, dd, h, m, s, tz);

   days    = YMDtoDays    (yy, mm, dd);
   seconds = HMStoSeconds (h, m, s, tz);
   normalize();
}

DateTime::DateTime(std::string str)
{
   int yy, mm, dd, h, m, s, tz;
   stringToComps(str, yy, mm, dd, h, m, s, tz);
   days    = YMDtoDays(yy, mm, dd);
   seconds = HMStoSeconds(h,m,s,tz);
   normalize();
}

//------------------
void DateTime::getYMD(int &y, int &m, int &d) const
{
   int rest = getDays();
   y = 2000;
   int diy = daysInYear(y);
   while (rest > diy)
   {
      ++y;
      rest -= diy;
      diy = daysInYear(y);
   }

   m = 1;
   int dim = daysInMonth(m, y);
   while (rest > dim)
   {
      rest -= dim;
      ++m;
      dim = daysInMonth(m, y);
   }
   d = rest + 1;
}

//------------------
void DateTime::getHMS(int &h, int &m, int &s) const
{
   int rest = getSeconds();

   h = rest / (60 * 60);
   rest = rest % (60 * 60);
   m = rest / 60;
   s = rest % 60;
}

//------------------
void DateTime::addSeconds(int seconds)
{
   this->seconds += seconds;
   normalize();
}

//------------------
void DateTime::addDST(int dstValue)
{
   if (dstValue < 0 || dstValue > 2)
      dstValue = 0;
   addSeconds(60 * 15 * dstValue);
}

//------------------
void DateTime::addDays(int days)
{
   this->days += days;
}

//------------------
int DateTime::compareTo(const DateTime &other)
{

   if (days < other.getDays())
      return -1;
   if (days > other.getDays())
      return 1;

   return compareTimeTo(other);
}

//------------------
int DateTime::compareTimeTo(const DateTime &other)
{

   if (seconds < other.getSeconds())
      return -1;
   if (seconds > other.getSeconds())
      return 1;

   return 0;
}

std::string DateTime::toString(bool doDate, bool doTime)
{
   std::stringstream ss;

   if (doDate)
   {
      int yy, mm, dd;
      getYMD(yy, mm, dd);
      ss << dd;
      ss << ' ';
      ss << monthName(mm);
      ss << ' ';
      ss << yy;
   }
   if (doTime)
   {
      if (doDate)
         ss << "  ";
      int h, m, s;
      getHMS(h, m, s);

      if (h < 10)
         ss << " ";
      ss << h;
      ss << ':';
      ss << m;
      ss << ':';
      ss << s;
   }
   return ss.str();
}

//---------------
static void divmod(int x, int y, int &quo, int &mod)
// divmod always yields a positive modulo
// divmod (-1, 3, a, b): a = -1, b = 2;
{
   quo = x / y;
   mod = x - y * quo;
   if (mod < 0)
   {
      mod += y; // make positive
      --quo;
   }
}

//---------------------------
void DateTime::normalize()
// bring each of the components of a date-time in range
{
   const int secsPerDay = 24 * 60 * 60;

   int quot = seconds / secsPerDay;
   divmod(seconds, secsPerDay, quot, seconds);
   days += quot;
}