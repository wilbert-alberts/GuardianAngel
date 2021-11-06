/*
 * AngelFactory.hpp
 *
 *  Created on: 5 okt. 2021
 *      Author: wilbert
 */

#ifndef LIB_ANGEL_ANGELFACTORY_HPP_
#define LIB_ANGEL_ANGELFACTORY_HPP_

#include <memory>

class Angel;
class WatchDog;
class ActivityDetector;
class GSM;

namespace AngelFactory {

std::shared_ptr<Angel> create(
		const std::string& phoneNr,
		std::shared_ptr<WatchDog> wd,
		std::shared_ptr<ActivityDetector> hr,
		std::shared_ptr<GSM> gsm);
}



#endif /* LIB_ANGEL_ANGELFACTORY_HPP_ */
