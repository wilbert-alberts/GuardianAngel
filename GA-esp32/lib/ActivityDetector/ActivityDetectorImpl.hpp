/*
 * ActivityDetectorImpl.hpp
 *
 *  Created on: 22 nov. 2021
 *      Author: wilbert
 */

#ifndef LIB_ACTIVITYDETECTOR_ACTIVITYDETECTORIMPL_HPP_
#define LIB_ACTIVITYDETECTOR_ACTIVITYDETECTORIMPL_HPP_

#include <Debouncer.hpp>
#include <IActivityDetector.hpp>
#include <ITicking.hpp>
#include <ValueProvider.hpp>

class ActivityDetectorImpl: public IActivityDetector, public ITicking {
public:
	ActivityDetectorImpl();
	virtual ~ActivityDetectorImpl() {
	}
	virtual PeriodicTask* createTask();
	virtual void tick();
	virtual void clearActivity();
	virtual int getNrActivations();

private:
	// Handle detected
	int nrActivations;

	Debouncer activityDebouncer;
};






#endif /* LIB_ACTIVITYDETECTOR_ACTIVITYDETECTORIMPL_HPP_ */
