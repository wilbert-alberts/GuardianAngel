/*
 * Observable.h
 *
 *  Created on: 30 apr. 2019
 *      Author: wilbert
 */

#ifndef OBSERVABLE_H_
#define OBSERVABLE_H_

#include <stdint.h>


class Observer
{
public:
	virtual ~Observer();
	virtual void notify() = 0;
};

class Observable {
public:
	static Observable* create(uint8_t size);
	virtual ~Observable() {};

	virtual void registerObserver(Observer* obs) = 0;
	virtual void deregisterObserver(Observer* obs) = 0;
	virtual void notifyAll() = 0;

};

#endif /* OBSERVABLE_H_ */
