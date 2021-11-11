/*
 * IAlarmProcessor.hpp
 *
 *  Created on: 8 nov. 2021
 *      Author: wilbert
 */

#ifndef INCLUDE_IALARMPROCESSOR_HPP_
#define INCLUDE_IALARMPROCESSOR_HPP_

#include <string>

class IAlarmProcessor {
public:
	virtual ~IAlarmProcessor() {};
	virtual void sendAlarm(const std::string& phoneNr) = 0;
};


#endif /* INCLUDE_IALARMPROCESSOR_HPP_ */
