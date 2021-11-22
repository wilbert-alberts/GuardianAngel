/*
 * ConfigFactory.hpp
 *
 *  Created on: 14 sep. 2021
 *      Author: wilbert
 */

#ifndef CONFIGFACTORY_HPP_
#define CONFIGFACTORY_HPP_


#include <memory>

class IConfigProvider;

namespace ConfigFactory {

extern std::shared_ptr<IConfigProvider> create();

}

#endif /* CONFIGFACTORY_HPP_ */
