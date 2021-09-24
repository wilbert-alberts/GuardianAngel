/*
 * Time24Factory.hpp
 *
 *  Created on: 19 sep. 2021
 *      Author: wilbert
 */

#ifndef TIME24FACTORY_HPP_
#define TIME24FACTORY_HPP_

#include<memory>

class Time24;

namespace Time24Factory {

std::shared_ptr<Time24> create(int h, int m, int s);

}
#endif /* TIME24FACTORY_HPP_ */
