/*
 * AngelMgrImpl.hpp
 *
 *  Created on: 17 nov. 2021
 *      Author: wilbert
 */

#ifndef LIB_ANGEL_ANGELMGRIMPL_HPP_
#define LIB_ANGEL_ANGELMGRIMPL_HPP_

#include <AngelMgr.hpp>
#include <IBtnListener.hpp>
#include <memory>
#include <string>
#include <vector>

class Angel;
class IMessage;
class IMessageProvider;


class AngelMgrImpl: public AngelMgr, public IBtnListener {
public:
	virtual ~AngelMgrImpl() {
	}

	virtual void resetAngels();
	virtual void setHelpButton(std::shared_ptr<IButton> helpButton);
	virtual void setMessageProvider(
			std::shared_ptr<IMessageProvider> messageProvider);
	virtual void setActivityDetector(
			std::shared_ptr<IActivityDetector> activityDetector);
	virtual void setAlarmProcessor(
			std::shared_ptr<IAlarmProcessor> alarmProcessor);
	virtual void setTimeProvider(std::shared_ptr<ITimeProvider> timeProvider);
	virtual void setConfigProvider(std::shared_ptr<IConfigProvider> configMgr);
	virtual void btnPressed();

	virtual void doTick();

private:
	std::vector<std::shared_ptr<Angel>> angels;
	std::vector<std::shared_ptr<Angel>> alarmedAngels;

	std::shared_ptr<IAlarmProcessor> alarmProcessor;
	std::shared_ptr<IMessageProvider> messageProvider;
	std::shared_ptr<ITimeProvider> clock;
	std::shared_ptr<IActivityDetector> activityDetector;
	std::shared_ptr<IConfigProvider> configProvider;

	void processAngels();
	void processMessages();
	void processMessage(std::shared_ptr<IMessage> msg);
	void subscribeAngel(const std::string &phonenr, const std::string &start,
			const std::string &end, bool save);
	void unsubscribeAngel(const std::string &phonenr, const std::string &start,
			const std::string &end, bool save);
	std::shared_ptr<Angel> findAngel(const std::string &phoneNr);
	std::shared_ptr<Angel> addAngel(const std::string &phoneNr);
	void delAngel(const std::string &phoneNr);
	void saveConfig();
	void loadConfig();
};





#endif /* LIB_ANGEL_ANGELMGRIMPL_HPP_ */
