/*
 * Observable.cpp
 *
 *  Created on: 30 apr. 2019
 *      Author: wilbert
 */

#include "Observable.h"
#include <stdlib.h>

#define OBSERVER_SIZE (8)


class ObserverableImp: public Observable
{
public:
	ObserverableImp(int size);
	virtual ~ObserverableImp();
	virtual void registerObserver(Observer* obs);
	virtual void deregisterObserver(Observer* obs);
	virtual void notifyAll();

private:
	Observer** observers;
	uint8_t    nrObservers;
};

Observable* Observable::create(uint8_t size)
{
	return new ObserverableImp(size);
}

inline ObserverableImp::ObserverableImp(int size) :
	observers(new Observer*[OBSERVER_SIZE]), nrObservers(size) {
}

inline ObserverableImp::~ObserverableImp() {
}

inline void ObserverableImp::registerObserver(Observer* obs) {
	for (int i = 0; i < nrObservers; i++) {
		if (!observers[i]) {
			observers[i] = obs;
			return;
		}
	}
}

inline void ObserverableImp::deregisterObserver(Observer* obs) {
	for (int i = 0; i < nrObservers; i++) {
		if (observers[i] == obs) {
			observers[i] = NULL;
			return;
		}
	}
}

inline void ObserverableImp::notifyAll() {
	for (int i = 0; i < nrObservers; i++) {
		if (!observers[i]) {
			observers[i]->notify();
		}
	}
}
