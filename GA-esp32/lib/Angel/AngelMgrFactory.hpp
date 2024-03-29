/*
 * AngelMgrFactory.hpp
 *
 *  Created on: 5 okt. 2021
 *      Author: wilbert
 */

#ifndef LIB_ANGEL_ANGELMGRFACTORY_HPP_
#define LIB_ANGEL_ANGELMGRFACTORY_HPP_

#include <memory>

class PeriodicTask;

class AngelMgr;

namespace AngelMgrFactory {
std::shared_ptr<AngelMgr> create();
}

#endif /* LIB_ANGEL_ANGELMGRFACTORY_HPP_ */
