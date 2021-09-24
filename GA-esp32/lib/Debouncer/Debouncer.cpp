/*
 * Debouncer.cpp
 *
 *  Created on: 24 sep. 2021
 *      Author: wilbert
 */

#include "Debouncer.hpp"

Debouncer::Debouncer(int stabilizationPeriod, ValueProvider vp) :
		valueProvider(vp), stabilizationPeriod(stabilizationPeriod) {
	stableValue = valueProvider();
	unstableValue = stableValue;
	stablePeriods = 0;
}

int Debouncer::getValue() const {
	return stableValue;
}

bool Debouncer::tick() {
	int v = valueProvider();
	if (v != unstableValue) {
		stablePeriods = 0;
		unstableValue = v;
		return false;
	} else {
		if (stablePeriods < stabilizationPeriod) {
			stablePeriods++;
			return false;
		} else {
			if (stableValue != unstableValue) {
				stableValue = unstableValue;
				return true;
			} else {
				stableValue = unstableValue;
				return false;
			}
		}
	}
}
