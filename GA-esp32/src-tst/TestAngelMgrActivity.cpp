/*
 * TestAngelMgr.cpp
 *
 *  Created on: 18 nov. 2021
 *      Author: wilbert
 */

#include <AngelMgrFactory.hpp>
#include <AngelMgrImpl.hpp>
#include <gtest/gtest.h>
#include <memory>
#include <string>

#include "DbgActivityDetector.hpp"
#include "DbgAlarmProcessor.hpp"
#include "DbgClock.hpp"
#include "DbgConfigProvider.hpp"
#include "DbgMessageProvider.hpp"

TEST(AngelMgrActivity, NoAlarmNeeded) {
	auto mgr = AngelMgrFactory::create();
	auto tickMgr = (AngelMgrImpl*) mgr.get();

	auto dbgClock = new DbgClock();
	dbgClock->pushTime("7:00");
	dbgClock->pushTime("8:30");
	dbgClock->pushTime("10:30");

	auto dbgMessageProvider = new DbgMessageProvider();
	dbgMessageProvider->pushMessage(1, "0649882351", "subscribe 8:00 - 10:00");

	auto dbgConfigProvider = new DbgConfigProvider();
	dbgConfigProvider->ga_ini = "";

	auto dbgActivityDetector = new DbgActivityDetector();
	dbgActivityDetector->pushActivity(1);
	dbgActivityDetector->pushActivity(1);
	dbgActivityDetector->pushActivity(1);

	auto dbgAlarmProcessor = new DbgAlarmProcessor();

	mgr->setTimeProvider(std::shared_ptr<ITimeProvider>(dbgClock));
	mgr->setConfigProvider(std::shared_ptr<IConfigProvider>(dbgConfigProvider));
	mgr->setMessageProvider(
			std::shared_ptr<IMessageProvider>(dbgMessageProvider));
	mgr->setActivityDetector(
			std::shared_ptr<IActivityDetector>(dbgActivityDetector));
	mgr->setAlarmProcessor(std::shared_ptr<IAlarmProcessor>(dbgAlarmProcessor));

	tickMgr->tick();
	dbgClock->advanceTime();
	auto nrAngels = *dbgConfigProvider->getProperty("nr_angels");
	ASSERT_EQ(nrAngels.compare("1"), 0);

	ASSERT_FALSE(dbgAlarmProcessor->alarmSent());

	tickMgr->tick();
	dbgClock->advanceTime();
	ASSERT_FALSE(dbgAlarmProcessor->alarmSent());

	tickMgr->tick();
	dbgClock->advanceTime();
	ASSERT_FALSE(dbgAlarmProcessor->alarmSent());
}

TEST(AngelMgrActivity, AlarmNeeded) {
	auto mgr = AngelMgrFactory::create();
	auto tickMgr = (AngelMgrImpl*) mgr.get();

	auto dbgClock = new DbgClock();
	dbgClock->pushTime("7:00");
	dbgClock->pushTime("8:30");
	dbgClock->pushTime("10:30");

	auto dbgMessageProvider = new DbgMessageProvider();
	dbgMessageProvider->pushMessage(2, "0612345678", "subscribe 12:00 - 13:00");
	dbgMessageProvider->pushMessage(1, "0649882351", "subscribe 8:00 - 10:00");
	dbgMessageProvider->pushMessage(3, "0687654321", "subscribe 20:00 - 22:00");

	auto dbgConfigProvider = new DbgConfigProvider();
	dbgConfigProvider->ga_ini = "";

	auto dbgActivityDetector = new DbgActivityDetector();
	dbgActivityDetector->pushActivity(0);
	dbgActivityDetector->pushActivity(0);
	dbgActivityDetector->pushActivity(0);

	auto dbgAlarmProcessor = new DbgAlarmProcessor();

	mgr->setTimeProvider(std::shared_ptr<ITimeProvider>(dbgClock));
	mgr->setConfigProvider(std::shared_ptr<IConfigProvider>(dbgConfigProvider));
	mgr->setMessageProvider(
			std::shared_ptr<IMessageProvider>(dbgMessageProvider));
	mgr->setActivityDetector(
			std::shared_ptr<IActivityDetector>(dbgActivityDetector));
	mgr->setAlarmProcessor(std::shared_ptr<IAlarmProcessor>(dbgAlarmProcessor));

	tickMgr->tick();
	dbgClock->advanceTime();
	auto nrAngels = *dbgConfigProvider->getProperty("nr_angels");
	ASSERT_EQ(nrAngels.compare("3"), 0);

	ASSERT_FALSE(dbgAlarmProcessor->alarmSent());

	tickMgr->tick();
	dbgClock->advanceTime();
	ASSERT_FALSE(dbgAlarmProcessor->alarmSent());

	tickMgr->tick();
	dbgClock->advanceTime();
	ASSERT_TRUE(dbgAlarmProcessor->alarmSent());

	auto alarmRecipients =dbgAlarmProcessor->getAlarmRecipients();
	ASSERT_EQ(alarmRecipients.size(), (size_t)1);
	auto rec = std::find(alarmRecipients.begin(), alarmRecipients.end(), "0649882351");
	ASSERT_TRUE(rec != alarmRecipients.end());
}
