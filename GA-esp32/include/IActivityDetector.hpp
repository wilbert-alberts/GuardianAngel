/*
 * IActivityDetector.hpp
 *
 *  Created on: 8 nov. 2021
 *      Author: wilbert
 */

#ifndef INCLUDE_IACTIVITYDETECTOR_HPP_
#define INCLUDE_IACTIVITYDETECTOR_HPP_
class PeriodicTask;

class IActivityDetector {
public:
	virtual ~IActivityDetector() {}
	virtual PeriodicTask* createTask() = 0;
	virtual void clearActivity()=0;
	virtual int getNrActivations()=0;
};

#endif /* INCLUDE_IACTIVITYDETECTOR_HPP_ */
