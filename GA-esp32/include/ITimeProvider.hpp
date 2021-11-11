/*
 * ITimeProvider.hpp
 *
 *  Created on: 8 nov. 2021
 *      Author: wilbert
 */

#ifndef INCLUDE_ITIMEPROVIDER_HPP_
#define INCLUDE_ITIMEPROVIDER_HPP_

#include <memory>

class Time24;

class ITimeProvider {
public:
	virtual ~ITimeProvider() {};
	virtual std::shared_ptr<Time24> getTime() const= 0;
};


#endif /* INCLUDE_ITIMEOFDAYPROVIDER_HPP_ */
