/*
 * DbgAlarmProcessor.cpp
 *
 *  Created on: 2 dec. 2021
 *      Author: wilbert
 */

#include "DbgAlarmProcessor.hpp"
DbgAlarmProcessor::DbgAlarmProcessor() {
}

void DbgAlarmProcessor::sendAlarm(const std::string &phoneNr) {
	alarmRecipients.push_back(phoneNr);
}

void DbgAlarmProcessor::resetAlarm() {
	alarmRecipients.clear();
}

bool DbgAlarmProcessor::alarmSent() {
	return alarmRecipients.size() > 0;
}

std::vector<std::string> DbgAlarmProcessor::getAlarmRecipients() {
	return alarmRecipients;
}

