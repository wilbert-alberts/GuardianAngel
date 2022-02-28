/*
 * ActivityDetectorFactory.hpp
 *
 *  Created on: 24 sep. 2021
 *      Author: wilbert
 */

#ifndef LIB_ACTIVITYDETECTOR_ACTIVITYDETECTORFACTORY_HPP_
#define LIB_ACTIVITYDETECTOR_ACTIVITYDETECTORFACTORY_HPP_

#include <ValueProvider.hpp>
#include <memory>

class PeriodicTask;

class IActivityDetector;

namespace ActivityDetectorFactory {
	std::shared_ptr<IActivityDetector> create();
}

#endif /* LIB_ACTIVITYDETECTOR_ACTIVITYDETECTORFACTORY_HPP_ */
