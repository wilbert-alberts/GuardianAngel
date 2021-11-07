/*
 * ConfigFactory.hpp
 *
 *  Created on: 14 sep. 2021
 *      Author: wilbert
 */

#ifndef CONFIGFACTORY_HPP_
#define CONFIGFACTORY_HPP_

#include "Config.hpp"

#include <memory>

namespace ConfigFactory {

extern std::shared_ptr<Config> create();

}

#endif /* CONFIGFACTORY_HPP_ */
