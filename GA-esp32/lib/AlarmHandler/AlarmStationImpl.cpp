/*
 * AlarmStation.cpp
 *
 *  Created on: 8 nov. 2021
 *      Author: wilbert
 */

#include "AlarmStation.hpp"
#include "AlarmStationFactory.hpp"
#include "IMessageSender.hpp"

#include <memory>

class AlarmStationImpl: public AlarmStation {
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

namespace AlarmStationFactory {

std::shared_ptr<AlarmStation> create() {
	return std::shared_ptr<AlarmStation>(new AlarmStationImpl());
}

}

AlarmStationImpl::AlarmStationImpl() {
	alarmMsg = "VIP requested help! Please respond as quick as possible.";
}

AlarmStationImpl::~AlarmStationImpl() {
}

void AlarmStationImpl::setMessageSender(
		std::shared_ptr<IMessageSender> sender) {
	msgSender = sender;
}

void AlarmStationImpl::sendAlarm(const std::string &phoneNr) {
	if (msgSender != nullptr) {
		msgSender->sendMessage(phoneNr, alarmMsg);
	}
}

void AlarmStationImpl::setAlarmMessage(const std::string &msg) {
	alarmMsg = msg;
}
