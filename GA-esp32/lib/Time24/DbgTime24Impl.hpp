/*
 * Time24Impl.cpp
 *
 *  Created on: 19 sep. 2021
 *      Author: wilbert
 */
#ifndef TIME24DBGIMPL_HPP_
#define TIME24DBGIMPL_HPP_

#include <Time24Impl.hpp>
#include <iostream>

class DbgTime24Impl: public Time24Impl {
public:
	DbgTime24Impl(int h, int m, int s) :
			Time24Impl(h, m, s) {
		std::cerr << "#Time24 (+)" << ++nrInstances << std::endl;
	}
	virtual ~DbgTime24Impl() {
		std::cerr << "#Time24 (-)" << --nrInstances << std::endl;
	}
private:
	static int nrInstances;
};

int DbgTime24Impl::nrInstances = 0;

#endif
