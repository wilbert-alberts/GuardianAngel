/*
 * AlarmStation.h
 *
 *  Created on: 8 nov. 2021
 *      Author: wilbert
 */

#ifndef LIB_ALARMHANDLER_ALARMSTATION_HPP_
#define LIB_ALARMHANDLER_ALARMSTATION_HPP_

#include <memory>

#include "IAlarmProcessor.hpp"

class IMessageSender;

class AlarmStation: public IAlarmProcessor {
public:
	virtual ~AlarmStation() {};
	virtual void setMessageSender(std::shared_ptr<IMessageSender> sender) = 0;
	virtual void setAlarmMessage(const std::string& alarmMsg) = 0;
};



#endif /* LIB_ALARMHANDLER_ALARMSTATION_HPP_ */
