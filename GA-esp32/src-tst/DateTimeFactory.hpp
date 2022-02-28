/*
 * DateTimeFactory.hpp
 *
 *  Created on: 4 Dec. 2021
 *      Stolen from: wilbert
 */

#ifndef DATETIMEFACTORY_HPP_
#define DATETIMEFACTORY_HPP_

#include <memory>

class DateTime;

namespace DateTimeFactory {

std::shared_ptr<DateTime> create (int year, int mon, int day, int h, int m, int s);
std::shared_ptr<DateTime> create (std::string ymdhms);

}
#endif /* DATETIMEFACTORY_HPP_ */