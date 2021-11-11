/*
 * Button.hpp
 *
 *  Created on: 8 nov. 2021
 *      Author: wilbert
 */

#ifndef INCLUDE_IBUTTON_HPP_
#define INCLUDE_IBUTTON_HPP_

#include <memory>

class IBtnListener;

class IButton
{
public:
	virtual ~IButton();
	virtual void addListener(std::shared_ptr<IBtnListener> listener);
	virtual void delListener(std::shared_ptr<IBtnListener>listener);
};


#endif /* INCLUDE_IBUTTON_HPP_ */
