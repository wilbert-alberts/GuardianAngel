/*
 * ActivityDetector.hpp
 *
 *  Created on: 24 sep. 2021
 *      Author: wilbert
 */

#ifndef LIB_ACTIVITYDETECTOR_ACTIVITYDETECTOR_HPP_
#define LIB_ACTIVITYDETECTOR_ACTIVITYDETECTOR_HPP_

#include <memory>

#include "platform.hpp"
#include "ValueProvider.hpp"

class ActivityListener {
public:
	virtual ~ActivityListener() {};
	virtual void activityDetected() = 0;
};

class ActivityDetector {
public:
	virtual ~ActivityDetector() {}
	virtual void setListener(std::shared_ptr<ActivityListener> listener) = 0;
};

#endif /* LIB_ACTIVITYDETECTOR_ACTIVITYDETECTOR_HPP_ */

