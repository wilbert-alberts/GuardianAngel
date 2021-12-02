/*
 * ITicking.hpp
 *
 *  Created on: 2 dec. 2021
 *      Author: wilbert
 */

#ifndef INCLUDE_ITICKING_HPP_
#define INCLUDE_ITICKING_HPP_



class ITicking {
public:
	virtual ~ITicking(){}
    virtual void tick()=0;
};



#endif /* INCLUDE_ITICKING_HPP_ */
