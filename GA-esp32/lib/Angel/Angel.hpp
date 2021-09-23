#ifndef ANGEL_HPP_
#define ANGEL_HPP_

#include <string>

class Angel
{
public:
    virtual ~Angel() {};
    virtual std::string getPhoneNumber() = 0;


    std::string name;
    std::vector<WatchInterval> watchIntervals;
};

#endif