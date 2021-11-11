/*
 * AlarmStationFactory.hpp
 *
 *  Created on: 8 nov. 2021
 *      Author: wilbert
 */

#ifndef LIB_ALARMHANDLER_ALARMSTATIONFACTORY_HPP_
#define LIB_ALARMHANDLER_ALARMSTATIONFACTORY_HPP_

#include <memory>

class AlarmStation;

namespace AlarmStationFactory {
extern std::shared_ptr<AlarmStation> create();
}

#endif /* LIB_ALARMHANDLER_ALARMSTATIONFACTORY_HPP_ */
