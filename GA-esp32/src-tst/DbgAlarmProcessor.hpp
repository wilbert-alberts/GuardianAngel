/*
 * DbgAlarmProcessor.hpp
 *
 *  Created on: 2 dec. 2021
 *      Author: wilbert
 */

#ifndef SRC_TST_DBGALARMPROCESSOR_HPP_
#define SRC_TST_DBGALARMPROCESSOR_HPP_

#include <IAlarmProcessor.hpp>
#include <string>
#include <vector>


class DbgAlarmProcessor: public IAlarmProcessor {
public:
	DbgAlarmProcessor();
	virtual ~DbgAlarmProcessor() {};
	virtual void sendAlarm(const std::string& phoneNr);

	void resetAlarm();
	bool alarmSent();
	std::vector<std::string> getAlarmRecipients();

private:
	std::vector<std::string> alarmRecipients;
};

#endif /* SRC_TST_DBGALARMPROCESSOR_HPP_ */
