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
#include <LoadableAngel.hpp>
#include <stddef.h>
#include <SaveableAngel.hpp>
#include <Time24.hpp>
#include <algorithm>
#include <iostream>
#include <iterator>

void AngelMgrImpl::resetAngels() {
	std::for_each(alarmedAngels.begin(), alarmedAngels.end(), [&](auto a) {
		angels.push_back(a);
	});
	alarmedAngels.clear();
}

void AngelMgrImpl::setHelpButton(std::shared_ptr<IButton> helpButton) {
	helpButton->addListener(std::shared_ptr<IBtnListener>(this));
}

void AngelMgrImpl::btnPressed() {
	if (alarmProcessor != nullptr) {
		// Send alarm to every angel
		std::for_each(angels.begin(), angels.end(), [&](auto a) {
			alarmProcessor->sendAlarm(a->getPhoneNr());
		});
		std::for_each(alarmedAngels.begin(), alarmedAngels.end(), [&](auto a) {
			alarmProcessor->sendAlarm(a->getPhoneNr());
		});
	}
}

void AngelMgrImpl::setMessageProvider(
		std::shared_ptr<IMessageProvider> provider) {
	messageProvider = provider;
}

void AngelMgrImpl::setActivityDetector(
		std::shared_ptr<IActivityDetector> detector) {
	activityDetector = detector;
}

void AngelMgrImpl::setAlarmProcessor(
		std::shared_ptr<IAlarmProcessor> processor) {
	alarmProcessor = processor;
}

void AngelMgrImpl::setTimeProvider(
		std::shared_ptr<ITimeProvider> timeProvider) {
	clock = timeProvider;
}

void AngelMgrImpl::setConfigProvider(
		std::shared_ptr<IConfigProvider> configMgr) {
	configProvider = configMgr;
	loadConfig();
}

void AngelMgrImpl::tick() {
	processAngels();
	processMessages();
}

void AngelMgrImpl::processAngels() {
	// Retrieve time
	auto now = clock->getTime();
	// Retrieve and clear activations
	auto nrActivations = activityDetector->getNrActivations();
	activityDetector->clearActivity();

	// Determine angels that have not yet been alarmed
	std::vector<std::shared_ptr<Angel>> activeAngels;
	std::for_each(angels.begin(), angels.end(), [&](auto a) {
		if (!(a->helpNeeded())) {
			activeAngels.push_back(a);
		}
	});

	// Tell angels that have not yet been alarmed
	std::for_each(activeAngels.begin(), activeAngels.end(), [&](auto a) {
		a->timeProgress(now);
		if (nrActivations > 0)
			a->activityDetected();
	});

	// Send alarm to angels when needed
	if (alarmProcessor != nullptr) {
		std::for_each(activeAngels.begin(), activeAngels.end(), [&](auto a) {
			if (a->helpNeeded()) {
				alarmProcessor->sendAlarm(a->getPhoneNr());
				alarmedAngels.push_back(a);
			}
		});
	}
}

void AngelMgrImpl::processMessages() {
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
}

void AngelMgrImpl::processMessage(std::shared_ptr<IMessage> msg) {
	if (msg->getAction() == "subscribe") {
		subscribeAngel(msg->getSender(), msg->getStart(), msg->getEnd(), true);
	}
	if (msg->getAction() == "unsubscribe") {
		unsubscribeAngel(msg->getSender(), msg->getStart(), msg->getEnd(),
				true);
	}
}

std::shared_ptr<Angel> AngelMgrImpl::findAngel(const std::string &phoneNr) {
	auto angelInAngels = std::find_if(angels.begin(), angels.end(),
			[&](std::shared_ptr<Angel> a) {
				return a->getPhoneNr() == phoneNr;
			});
	if (angelInAngels != angels.end())
		return *angelInAngels;
	auto angelInAlarmedAngels = std::find_if(alarmedAngels.begin(),
			alarmedAngels.end(), [&](std::shared_ptr<Angel> a) {
				return a->getPhoneNr() == phoneNr;
			});
	if (angelInAlarmedAngels != alarmedAngels.end())
		return *angelInAlarmedAngels;
	return nullptr;
}

std::shared_ptr<Angel> AngelMgrImpl::addAngel(const std::string &phoneNr) {
	auto newAngel = AngelFactory::create(phoneNr);
	angels.push_back(newAngel);
	return newAngel;
}

void AngelMgrImpl::delAngel(const std::string &phoneNr) {
	auto newAngelEnd = std::remove_if(angels.begin(), angels.end(),
			[&](auto a) {
				return a->getPhoneNr() == phoneNr;
			});
	angels.erase(newAngelEnd, angels.end());
	auto newAlarmedAngelEnd = std::remove_if(alarmedAngels.begin(),
			alarmedAngels.end(), [&](auto a) {
				return a->getPhoneNr() == phoneNr;
			});
	alarmedAngels.erase(newAlarmedAngelEnd, alarmedAngels.end());
}

void AngelMgrImpl::subscribeAngel(const std::string &phonenr,
		const std::string &start, const std::string &end, bool save) {

	auto angel = findAngel(phonenr);

	if (angel == nullptr) {
		auto newAngel = AngelFactory::create(phonenr);
		angels.push_back(newAngel);
		angel = newAngel;
	}

	angel->addInterval(start, end);
	saveConfig();
}

void AngelMgrImpl::unsubscribeAngel(const std::string &phonenr,
		const std::string &start, const std::string &end, bool save) {
	auto angel = findAngel(phonenr);
	if (angel != nullptr) {
		if (start != "" || end != "") {
			angel->delInterval(start, end);
			if (angel->getNrIntervals() == 0) {
				delAngel(phonenr);
			}
		} else {
			delAngel(phonenr);
		}
	}
	saveConfig();
}

void AngelMgrImpl::saveConfig() {
	if (configProvider) {
		configProvider->putProperty("nr_angels", std::to_string(angels.size()));
		for (size_t i = 0; i < angels.size(); i++) {
			auto sa = new SaveableAngel(angels[i], configProvider, i);
			sa->save();
		}
		configProvider->saveProperties();
	}
}

void AngelMgrImpl::loadConfig() {
	if (configProvider) {
		auto nrAngelsStr = configProvider->getProperty("nr_angels");
		int nrAngels = 0;
		if (nrAngelsStr != nullptr) {
			nrAngels = std::stoi(*nrAngelsStr);
		}
		for (int i = 0; i < nrAngels; i++) {
			auto newLAngel = new LoadableAngel(configProvider, i);
			auto newAngel = newLAngel->toAngel();
			angels.push_back(newAngel);
		}
	}
}
