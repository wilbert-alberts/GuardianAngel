/*
 * IBtnListener.hpp
 *
 *  Created on: 8 nov. 2021
 *      Author: wilbert
 */

#ifndef INCLUDE_IBTNLISTENER_HPP_
#define INCLUDE_IBTNLISTENER_HPP_

class IBtnListener {
public:
	virtual ~IBtnListener() {};
	virtual void btnPressed() = 0;
};



#endif /* INCLUDE_IBTNLISTENER_HPP_ */
