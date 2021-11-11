/*
 * IActivityDetector.hpp
 *
 *  Created on: 8 nov. 2021
 *      Author: wilbert
 */

#ifndef INCLUDE_IACTIVITYDETECTOR_HPP_
#define INCLUDE_IACTIVITYDETECTOR_HPP_

class IActivityDetector {
public:
	virtual ~IActivityDetector();
	virtual void clearActivity()=0;
	virtual int getNrActiviations()=0;
};

#endif /* INCLUDE_IACTIVITYDETECTOR_HPP_ */
