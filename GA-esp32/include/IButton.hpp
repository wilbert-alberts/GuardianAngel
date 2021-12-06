/*
 * Button.hpp
 *
 *  Created on: 8 nov. 2021
 *      Author: wilbert
 */

#ifndef INCLUDE_IBUTTON_HPP_
#define INCLUDE_IBUTTON_HPP_

#include <memory>
#include <IBtnListener.hpp>
class PeriodicTask;

class IButton
{
public:
	virtual ~IButton() {}
	virtual PeriodicTask* createTask() = 0;
	virtual void addListener(std::shared_ptr<IBtnListener> listener) = 0;
	virtual void delListener(std::shared_ptr<IBtnListener>listener) = 0;
};


#endif /* INCLUDE_IBUTTON_HPP_ */
