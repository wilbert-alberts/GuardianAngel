/*
 * ActivityDetectorFactory.hpp
 *
 *  Created on: 24 sep. 2021
 *      Author: wilbert
 */

#ifndef LIB_ACTIVITYDETECTOR_ACTIVITYDETECTORFACTORY_HPP_
#define LIB_ACTIVITYDETECTOR_ACTIVITYDETECTORFACTORY_HPP_

#include <memory>
#include "ValueProvider.hpp"

class IActivityDetector;

namespace ActivityDetectorFactory {
	std::shared_ptr<IActivityDetector> create(ValueProvider vp);
	std::shared_ptr<IActivityDetector> createTask(ValueProvider vp);
}

#endif /* LIB_ACTIVITYDETECTOR_ACTIVITYDETECTORFACTORY_HPP_ */
