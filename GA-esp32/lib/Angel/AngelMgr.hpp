/*
 * AngelMgr.hpp
 *
 *  Created on: 5 okt. 2021
 *      Author: wilbert
 */

#ifndef LIB_ANGEL_ANGELMGR_HPP_
#define LIB_ANGEL_ANGELMGR_HPP_

#include <memory>

class PeriodicTask;

class IConfigProvider;
class IMessageProvider;
class IButton;
class IMessageProcessor;
class IActivityDetector;
class IAlarmProcessor;
class ITimeProvider;

class AngelMgr
{
public:
	virtual ~AngelMgr() {}

	virtual PeriodicTask* createTask() = 0;
	virtual void resetAngels() = 0;
	virtual void setHelpButton(std::shared_ptr<IButton> helpButton) = 0;
	virtual void setMessageProvider(std::shared_ptr<IMessageProvider> messageProvider) = 0;
	virtual void setActivityDetector(std::shared_ptr<IActivityDetector> activityDetector) = 0;
	virtual void setAlarmProcessor(std::shared_ptr<IAlarmProcessor> alarmProcessor) = 0;
	virtual void setTimeProvider(std::shared_ptr<ITimeProvider> timeProvider) = 0;
	virtual void setConfigProvider(std::shared_ptr<IConfigProvider> configMgr) = 0;
};



#endif /* LIB_ANGEL_ANGELMGR_HPP_ */
