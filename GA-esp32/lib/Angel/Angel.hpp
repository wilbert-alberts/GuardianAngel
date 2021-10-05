#ifndef ANGEL_HPP_
#define ANGEL_HPP_

#include <string>

class Angel
{
public:
    virtual ~Angel() {};

    virtual const std::string& getPhoneNr() const = 0;
    virtual void addInterval(const std::string& start, const std::string& end) = 0;
    virtual void delInterval(const std::string& start, const std::string& end) = 0;
};

#endif
