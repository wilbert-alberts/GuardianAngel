/*
 * Time24Factory.hpp
 *
 *  Created on: 19 sep. 2021
 *      Author: wilbert
 */

#ifndef TIME24FACTORY_HPP_
#define TIME24FACTORY_HPP_

class Time24;

namespace Time24Factory {

extern Time24* create(int h, int m, int s = 0);

}
#endif /* TIME24FACTORY_HPP_ */
