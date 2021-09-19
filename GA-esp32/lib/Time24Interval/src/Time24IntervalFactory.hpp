/*
 * Time24IntervalFactory.hpp
 *
 *  Created on: 19 sep. 2021
 *      Author: wilbert
 */

#ifndef TIME24INTERVALFACTORY_HPP_
#define TIME24INTERVALFACTORY_HPP_

class Time24;
class Time24Interval;

namespace Time24IntervalFactory {

Time24Interval* create(const Time24 &start, const Time24 &end);

}

#endif /* TIME24INTERVALFACTORY_HPP_ */