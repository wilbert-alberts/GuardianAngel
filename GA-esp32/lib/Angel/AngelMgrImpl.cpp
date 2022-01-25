/*
 * AngelMgrImpl.cpp
 *
 *  Created on: 8 nov. 2021
 *      Author: wilbert
 */

#include <Angel.hpp>
#include <AngelFactory.hpp>
#include <AngelMgrImpl.hpp>
#include <IActivityDetector.hpp>
#include <IAlarmProcessor.hpp>
#include <IButton.hpp>
#include <IConfigProvider.hpp>
#include <IMessage.hpp>
#include <IMessageProvider.hpp>
#include <ITimeProvider.hpp>
#include <PeriodicTask.hpp>
#include <stddef.h>
#include <Time24.hpp>
#include "WatchInterval.hpp"
#include <algorithm>
#include <iterator>
#include <stdlib.h>
#include "ArduinoJson-v6.19.1.h"



PeriodicTask *AngelMgrImpl::createTask()
{
	return new PeriodicTask("angelMgr", std::shared_ptr<ITicking>(this), 60 * 1000, 8192);
}
void AngelMgrImpl::resetAngels()
{
	std::for_each(alarmedAngels.begin(), alarmedAngels.end(), [&](std::shared_ptr<Angel> a)
				  { angels.push_back(a); });
	alarmedAngels.clear();
}

void AngelMgrImpl::setHelpButton(std::shared_ptr<IButton> helpButton)
{
	helpButton->addListener(std::shared_ptr<IBtnListener>(this));
}

void AngelMgrImpl::btnPressed()
{
	if (alarmProcessor != nullptr)
	{
		// Send alarm to every angel
		std::for_each(angels.begin(), angels.end(), [&](std::shared_ptr<Angel> a)
					  { alarmProcessor->sendAlarm(a->getPhoneNr()); });
		std::for_each(alarmedAngels.begin(), alarmedAngels.end(), [&](std::shared_ptr<Angel> a)
					  { alarmProcessor->sendAlarm(a->getPhoneNr()); });
	}
}

void AngelMgrImpl::setMessageProvider(
	std::shared_ptr<IMessageProvider> provider)
{
	messageProvider = provider;
}

void AngelMgrImpl::setActivityDetector(
	std::shared_ptr<IActivityDetector> detector)
{
	activityDetector = detector;
}

void AngelMgrImpl::setAlarmProcessor(
	std::shared_ptr<IAlarmProcessor> processor)
{
	alarmProcessor = processor;
}

void AngelMgrImpl::setTimeProvider(
	std::shared_ptr<ITimeProvider> timeProvider)
{
	clock = timeProvider;
}

void AngelMgrImpl::setConfigProvider(
	std::shared_ptr<IConfigProvider> configMgr)
{
	configProvider = configMgr;
	loadConfig();
}

void AngelMgrImpl::tick()
{
	LOG_ENTRY();
	processAngels();
	processMessages();
	LOG_EXIT();
}

void AngelMgrImpl::processAngels()
{
	LOG_ENTRY();
	// Retrieve time
	auto now = clock->getTime();
	// Retrieve and clear activations
	auto nrActivations = activityDetector->getNrActivations();
	activityDetector->clearActivity();

	// Determine angels that have not yet been alarmed
	std::vector<std::shared_ptr<Angel>> activeAngels;
	std::for_each(angels.begin(), angels.end(), [&](std::shared_ptr<Angel> a)
				  {
		if (!(a->helpNeeded())) {
			activeAngels.push_back(a);
		} });

	// Tell angels that have not yet been alarmed
	std::for_each(activeAngels.begin(), activeAngels.end(), [&](std::shared_ptr<Angel> a)
				  {
		a->timeProgress(now);
		if (nrActivations > 0)
			a->activityDetected(); });

	// Send alarm to angels when needed
	if (alarmProcessor != nullptr)
	{
		std::for_each(activeAngels.begin(), activeAngels.end(), [&](std::shared_ptr<Angel> a)
					  {
			if (a->helpNeeded()) {
				alarmProcessor->sendAlarm(a->getPhoneNr());
				alarmedAngels.push_back(a);
			} });
	}
	LOG_EXIT();
}

void AngelMgrImpl::processMessages()
{
	LOG_ENTRY();

	// Retrieve messages
	auto msgs = messageProvider->getMessageIDs();
	// Process messages one by one
	std::for_each(msgs.begin(), msgs.end(), [&](MessageID mid) {
		auto msg = messageProvider->getMessage(mid);
		if (msg->isValid()) {
			processMessage(msg);
		}
		// Regards valid or not, remove message
		messageProvider->delMessage(msg->getMessageID()); 
	});
	LOG_EXIT();
}

void AngelMgrImpl::processMessage(std::shared_ptr<IMessage> msg)
{
	LOG_ENTRY();
	if (msg->getAction() == "subscribe")
	{
		subscribeAngel(msg->getSender(), msg->getStart(), msg->getEnd(), true);
	}
	if (msg->getAction() == "unsubscribe")
	{
		unsubscribeAngel(msg->getSender(), msg->getStart(), msg->getEnd(),
						 true);
	}
	LOG_EXIT();
}

std::shared_ptr<Angel> AngelMgrImpl::findAngel(const std::string &phoneNr)
{
	auto angelInAngels = std::find_if(angels.begin(), angels.end(),
									  [&](std::shared_ptr<Angel> a)
									  {
										  return a->getPhoneNr() == phoneNr;
									  });
	if (angelInAngels != angels.end())
		return *angelInAngels;
	auto angelInAlarmedAngels = std::find_if(alarmedAngels.begin(),
											 alarmedAngels.end(), [&](std::shared_ptr<Angel> a)
											 { return a->getPhoneNr() == phoneNr; });
	if (angelInAlarmedAngels != alarmedAngels.end())
		return *angelInAlarmedAngels;
	return nullptr;
}

std::shared_ptr<Angel> AngelMgrImpl::addAngel(const std::string &phoneNr)
{
	auto newAngel = AngelFactory::create(phoneNr);
	angels.push_back(newAngel);
	return newAngel;
}

void AngelMgrImpl::delAngel(const std::string &phoneNr)
{
	auto newAngelEnd = std::remove_if(angels.begin(), angels.end(),
									  [&](std::shared_ptr<Angel> a)
									  {
										  return a->getPhoneNr() == phoneNr;
									  });
	angels.erase(newAngelEnd, angels.end());
	auto newAlarmedAngelEnd = std::remove_if(alarmedAngels.begin(),
											 alarmedAngels.end(), [&](std::shared_ptr<Angel> a)
											 { return a->getPhoneNr() == phoneNr; });
	alarmedAngels.erase(newAlarmedAngelEnd, alarmedAngels.end());
}

void AngelMgrImpl::subscribeAngel(const std::string &phonenr,
								  const std::string &start, const std::string &end, bool save)
{
	LOG_ENTRY();
	auto angel = findAngel(phonenr);

	if (angel == nullptr)
	{
		auto newAngel = AngelFactory::create(phonenr);
		angels.push_back(newAngel);
		angel = newAngel;
	}

	angel->addInterval(start, end);
	saveConfig();
	LOG_EXIT();
}

void AngelMgrImpl::unsubscribeAngel(const std::string &phonenr,
									const std::string &start, const std::string &end, bool save)
{
	auto angel = findAngel(phonenr);
	if (angel != nullptr)
	{
		if (start != "" || end != "")
		{
			angel->delInterval(start, end);
			if (angel->getNrIntervals() == 0)
			{
				delAngel(phonenr);
			}
		}
		else
		{
			delAngel(phonenr);
		}
	}
	saveConfig();
}


void AngelMgrImpl::saveConfig()
{
	DynamicJsonDocument doc(4096);

	JsonObject root = doc.to<JsonObject>();
	root["nrAngels"] = angels.size();
	JsonArray angs = root.createNestedArray("angels");
	for (size_t i = 0; i < angels.size(); i++)
	{
		JsonObject ang = angs.createNestedObject();
		ang["phoneNr"] = angels[i]->getPhoneNr();
		ang["nrIntervals"] = angels[i]->getNrIntervals();
		JsonArray ivs = ang.createNestedArray("intervals");
		for (int j = 0; j < angels[i]->getNrIntervals(); j++)
		{
			JsonObject iv = ivs.createNestedObject();
			std::shared_ptr<WatchInterval> interval = angels[i]->getInterval(j);
			std::shared_ptr<const Time24> start = interval->getStart();
			std::shared_ptr<const Time24> end = interval->getEnd();
			iv["start"] = start->toString();
			iv["end"] = end->toString();
		}
	}

	std::string props;
	serializeJson(doc, props);

	if (configProvider)
	{
		configProvider->saveProperties(props);
	}
}

void AngelMgrImpl::loadConfig()
{
	angels.clear();

	if (configProvider)
	{
		std::string props;
		configProvider->loadProperties(props);

		DynamicJsonDocument doc(4096);
		deserializeJson(doc, props);

		int nrAngels = doc["nrAngels"];
		for (int i = 0; i < nrAngels; i++)
		{
			auto ang = doc["angels"][i];
			std::string phoneNr = ang["phoneNr"];

			int nrIntervals = ang["nrIntervals"];
			for (int j = 0; j < nrIntervals; j++)
			{
				auto iv = ang["intervals"][j];
				std::string start = iv["start"];
				std::string end = iv["end"];

				subscribeAngel(phoneNr, start, end, false);
			}
		}
	}
}
