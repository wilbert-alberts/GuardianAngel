/*
 * DbgActivityDetector.cpp
 *
 *  Created on: 22 nov. 2021
 *      Author: wilbert
 */

#include "DbgActivityDetector.hpp"

#include <algorithm>

void DbgActivityDetector::pushActivity(int nrActivations) {
	activations.push_back(nrActivations);
}

void DbgActivityDetector::pushIdlePeriods(int nrIdlePeriods) {
	while (nrIdlePeriods > 0) {
		activations.push_back(0);
		nrIdlePeriods--;
	}
}

void DbgActivityDetector::clearActivity() {
	if (activations.size() > 0)
		activations.erase(activations.begin());
}

int DbgActivityDetector::getNrActivations() {
	if (activations.size() > 0)
		return *activations.begin();
	else
		return 0;
}
