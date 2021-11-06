/*
 * AlarmHandler.h
 *
 *  Created on: 6 nov. 2021
 *      Author: wilbert
 */

#ifndef LIB_ALARMHANDLER_ALARMHANDLER_HPP_
#define LIB_ALARMHANDLER_ALARMHANDLER_HPP_

class AlarmHandler {
public:
	virtual void raiseAlarm() = 0;
	virtual ~AlarmHandler() {};
};

#endif /* LIB_ALARMHANDLER_ALARMHANDLER_HPP_ */
