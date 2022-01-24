//
// DateTime.hpp -- date time and derived conversions
#ifndef _DATETIME_HPP
#define _DATETIME_HPP

#include <memory>

class DateTime {
 public:
   DateTime () {days = 0; seconds = 0;}
   DateTime (int yy, int mm, int dd, int h, int m, int s, int dst = 0);
   DateTime (std::string str);
   ~DateTime () {}
   int  getDays         () const { return days;}
   int  getSeconds      () const { return seconds;}
   void getYMD          (int &y, int &m, int &d) const;
   void getHMS          (int &h, int &m, int &s) const;

   void addDays         (int days);
   void addSeconds      (int s);
   void addDST          (int dst);
   int  compareTo       (const DateTime &other);
   int  compareTimeTo   (const DateTime &other);
   std::string toString (const bool doDate = true, const bool doTime = true);

private:
   int  days;
   int  seconds;
   void normalize        ();
};

#endif // DateTime.hpp
