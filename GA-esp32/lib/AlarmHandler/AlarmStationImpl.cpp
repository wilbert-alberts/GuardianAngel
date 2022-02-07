/*
 * AlarmStation.cpp
 *
 *  Created on: 8 nov. 2021
 *      Author: wilbert
 */

#include "platform.hpp"

#include "AlarmStation.hpp"
#include "AlarmStationFactory.hpp"
#include "IMessageSender.hpp"

#include <memory>

class AlarmStationImpl : public AlarmStation
{
public:
	AlarmStationImpl();
	virtual ~AlarmStationImpl();
	virtual void setMessageSender(std::shared_ptr<IMessageSender> sender);
	virtual void setAlarmMessage(const std::string &alarmMsg);
	virtual void sendAlarm(const std::string &phoneNr);

private:
	std::shared_ptr<IMessageSender> msgSender;
	std::string alarmMsg;
};

namespace AlarmStationFactory
{

	std::shared_ptr<AlarmStation> create()
	{
		return std::shared_ptr<AlarmStation>(new AlarmStationImpl());
	}

}

AlarmStationImpl::AlarmStationImpl()
{
	LOG_ENTRY();
	setAlarmMessage("VIP requested help! Please respond as quick as possible.");
	LOG_EXIT();
}

AlarmStationImpl::~AlarmStationImpl()
{
	LOG_ENTRY();
	LOG_EXIT();
}

void AlarmStationImpl::setMessageSender(
	std::shared_ptr<IMessageSender> sender)
{
	LOG_ENTRY();
	msgSender = sender;
	LOG_EXIT();
}

void AlarmStationImpl::sendAlarm(const std::string &phoneNr)
{
	LOG_ENTRY("msgSender set %s, phoneNr: %s", boolToString(msgSender), phoneNr.c_str());
	if (msgSender != nullptr)
	{
		msgSender->sendMessage(phoneNr, alarmMsg);
	}
	LOG_EXIT();
}

void AlarmStationImpl::setAlarmMessage(const std::string &msg)
{
	LOG_ENTRY("msg: %s", msg.c_str());
	alarmMsg = msg;
	LOG_EXIT();
}
