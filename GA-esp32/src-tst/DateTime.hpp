//
// DateTime.hpp -- date time and derived conversions
#ifndef _DATETIME_HPP
#define _DATETIME_HPP

#include <memory>

class DateTime {
 public:
   virtual ~DateTime () {}
   virtual int getYear   () const = 0;
   virtual int getMonth  () const = 0;
   virtual int getDay    () const = 0;
   virtual int getHours  () const = 0;
	virtual int getMinutes() const = 0;
	virtual int getSeconds() const = 0;

   virtual void add         (int h, int m, int s) = 0;
   virtual void addDays     (int days) = 0;
   virtual void addHours    (int h) = 0;
   virtual void addMinutes  (int m) = 0;
   virtual void addSeconds  (int s) = 0;
   virtual void addDST      (int dstValue) = 0;
   virtual int  compareTo   (const DateTime &other) const = 0;
   virtual std::string toString (const bool doDate = true, const bool doTime = true) const = 0;
};
#endif // DateTime.hpp
