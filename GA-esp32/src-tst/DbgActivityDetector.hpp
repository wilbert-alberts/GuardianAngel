/*
 * DbgActivityDetector.hpp
 *
 *  Created on: 22 nov. 2021
 *      Author: wilbert
 */

#ifndef SRC_TST_DBGACTIVITYDETECTOR_HPP_
#define SRC_TST_DBGACTIVITYDETECTOR_HPP_

#include <IActivityDetector.hpp>
#include <memory>
#include <vector>

class ActivityListener;


class DbgActivityDetector: public IActivityDetector {
public:
	void pushActivity(int nrActivations);
	void pushIdlePeriods(int nrIdlePeriods);

	virtual ~DbgActivityDetector() {}

	virtual void clearActivity();
	virtual int getNrActivations();

private:

	std::vector<int> activations;
};


#endif /* SRC_TST_DBGACTIVITYDETECTOR_HPP_ */
