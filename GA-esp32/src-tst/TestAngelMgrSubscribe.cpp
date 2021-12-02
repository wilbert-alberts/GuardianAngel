/*
 * TestAngelMgr.cpp
 *
 *  Created on: 18 nov. 2021
 *      Author: wilbert
 */

#include <AngelMgrFactory.hpp>
#include <AngelMgrImpl.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <string>

#include "DbgActivityDetector.hpp"
#include "DbgClock.hpp"
#include "DbgConfigProvider.hpp"
#include "DbgMessageProvider.hpp"

TEST(AngelMgrSubscribe, NewSubscription) {
	auto mgr = AngelMgrFactory::create();
	auto tickMgr = (AngelMgrImpl*) mgr.get();

	auto dbgClock = new DbgClock();
	dbgClock->pushTime("7:00");

	auto dbgMessageProvider = new DbgMessageProvider();
	dbgMessageProvider->pushMessage(1, "0649882351", "subscribe 8:00 - 10:00");

	auto dbgConfigProvider = new DbgConfigProvider();
	dbgConfigProvider->ga_ini = "";

	auto dbgActivityDetector = new DbgActivityDetector();

	mgr->setTimeProvider(std::shared_ptr<ITimeProvider>(dbgClock));
	mgr->setConfigProvider(std::shared_ptr<IConfigProvider>(dbgConfigProvider));
	mgr->setMessageProvider(
			std::shared_ptr<IMessageProvider>(dbgMessageProvider));
	mgr->setActivityDetector(
			std::shared_ptr<IActivityDetector>(dbgActivityDetector));

	tickMgr->doTick();

	//	std::cerr << dbgConfigProvider->ga_ini << std::endl;
	ASSERT_EQ(dbgConfigProvider->ga_ini, "angel_0_interval_0_end=10:00:00\n"
			"angel_0_interval_0_start= 8:00:00\n"
			"angel_0_nrIntervals=1\n"
			"angel_0_phoneNr=0649882351\n"
			"nr_angels=1\n");

}

TEST(AngelMgrSubscribe, TwoAngelsSubscriptions) {
	auto mgr = AngelMgrFactory::create();
	auto tickMgr = (AngelMgrImpl*) mgr.get();

	auto dbgClock = new DbgClock();
	dbgClock->pushTime("7:00");

	auto dbgMessageProvider = new DbgMessageProvider();
	dbgMessageProvider->pushMessage(1, "0649882351", "subscribe 8:00 - 10:00");

	auto dbgConfigProvider = new DbgConfigProvider();
	dbgConfigProvider->ga_ini = "";

	auto dbgActivityDetector = new DbgActivityDetector();

	mgr->setTimeProvider(std::shared_ptr<ITimeProvider>(dbgClock));
	mgr->setConfigProvider(std::shared_ptr<IConfigProvider>(dbgConfigProvider));
	mgr->setMessageProvider(
			std::shared_ptr<IMessageProvider>(dbgMessageProvider));
	mgr->setActivityDetector(
			std::shared_ptr<IActivityDetector>(dbgActivityDetector));

	tickMgr->doTick();

	//	std::cerr << dbgConfigProvider->ga_ini << std::endl;
	ASSERT_EQ(dbgConfigProvider->ga_ini, "angel_0_interval_0_end=10:00:00\n"
			"angel_0_interval_0_start= 8:00:00\n"
			"angel_0_nrIntervals=1\n"
			"angel_0_phoneNr=0649882351\n"
			"nr_angels=1\n");

	dbgClock->pushTime("8:00");
	dbgMessageProvider->pushMessage(2, "0123456789", "subscribe 12:00 - 15:00");
	tickMgr->doTick();

//	std::cerr << dbgConfigProvider->ga_ini << std::endl;
	ASSERT_EQ(dbgConfigProvider->ga_ini, "angel_0_interval_0_end=10:00:00\n"
			"angel_0_interval_0_start= 8:00:00\n"
			"angel_0_nrIntervals=1\n"
			"angel_0_phoneNr=0649882351\n"
			"angel_1_interval_0_end=15:00:00\n"
			"angel_1_interval_0_start=12:00:00\n"
			"angel_1_nrIntervals=1\n"
			"angel_1_phoneNr=0123456789\n"
			"nr_angels=2\n");

}
TEST(AngelMgrSubscribe, TwoIntervalsSubscriptions) {
	auto mgr = AngelMgrFactory::create();
	auto tickMgr = (AngelMgrImpl*) mgr.get();

	auto dbgClock = new DbgClock();
	dbgClock->pushTime("7:00");

	auto dbgMessageProvider = new DbgMessageProvider();
	dbgMessageProvider->pushMessage(1, "0649882351", "subscribe 8:00 - 10:00");

	auto dbgConfigProvider = new DbgConfigProvider();
	dbgConfigProvider->ga_ini = "";

	auto dbgActivityDetector = new DbgActivityDetector();

	mgr->setTimeProvider(std::shared_ptr<ITimeProvider>(dbgClock));
	mgr->setConfigProvider(std::shared_ptr<IConfigProvider>(dbgConfigProvider));
	mgr->setMessageProvider(
			std::shared_ptr<IMessageProvider>(dbgMessageProvider));
	mgr->setActivityDetector(
			std::shared_ptr<IActivityDetector>(dbgActivityDetector));

	tickMgr->doTick();

	//	std::cerr << dbgConfigProvider->ga_ini << std::endl;
	ASSERT_EQ(dbgConfigProvider->ga_ini, "angel_0_interval_0_end=10:00:00\n"
			"angel_0_interval_0_start= 8:00:00\n"
			"angel_0_nrIntervals=1\n"
			"angel_0_phoneNr=0649882351\n"
			"nr_angels=1\n");

	dbgClock->pushTime("8:00");
	dbgMessageProvider->pushMessage(2, "0649882351", "subscribe 12:00 - 15:00");
	tickMgr->doTick();

//	std::cerr << dbgConfigProvider->ga_ini << std::endl;
	ASSERT_EQ(dbgConfigProvider->ga_ini, "angel_0_interval_0_end=10:00:00\n"
			"angel_0_interval_0_start= 8:00:00\n"
			"angel_0_interval_1_end=15:00:00\n"
			"angel_0_interval_1_start=12:00:00\n"
			"angel_0_nrIntervals=2\n"
			"angel_0_phoneNr=0649882351\n"
			"nr_angels=1\n");

}
TEST(AngelMgrSubscribe, UnSubscriptionOneInterval) {
	auto mgr = AngelMgrFactory::create();
	auto tickMgr = (AngelMgrImpl*) mgr.get();

	auto dbgClock = new DbgClock();
	dbgClock->pushTime("7:00");

	auto dbgMessageProvider = new DbgMessageProvider();
	dbgMessageProvider->pushMessage(1, "0649882351",
			"unsubscribe 8:00 - 10:00");

	auto dbgConfigProvider = new DbgConfigProvider();
	dbgConfigProvider->ga_ini = "angel_0_interval_0_end=10:00:00\n"
			"angel_0_interval_0_start= 8:00:00\n"
			"angel_0_interval_1_end=15:00:00\n"
			"angel_0_interval_1_start=12:00:00\n"
			"angel_0_nrIntervals=2\n"
			"angel_0_phoneNr=0649882351\n"
			"nr_angels=1\n";
	dbgConfigProvider->loadProperties();

	auto dbgActivityDetector = new DbgActivityDetector();

	mgr->setTimeProvider(std::shared_ptr<ITimeProvider>(dbgClock));
	mgr->setConfigProvider(std::shared_ptr<IConfigProvider>(dbgConfigProvider));
	mgr->setMessageProvider(
			std::shared_ptr<IMessageProvider>(dbgMessageProvider));
	mgr->setActivityDetector(
			std::shared_ptr<IActivityDetector>(dbgActivityDetector));

	tickMgr->doTick();

//	std::cerr << dbgConfigProvider->ga_ini << std::endl;
	ASSERT_EQ(dbgConfigProvider->ga_ini, "angel_0_interval_0_end=15:00:00\n"
			"angel_0_interval_0_start=12:00:00\n"
			"angel_0_interval_1_end=15:00:00\n"
			"angel_0_interval_1_start=12:00:00\n"
			"angel_0_nrIntervals=1\n"
			"angel_0_phoneNr=0649882351\n"
			"nr_angels=1\n");
}
TEST(AngelMgrSubscribe, UnSubscriptionLastInterval) {
	auto mgr = AngelMgrFactory::create();
	auto tickMgr = (AngelMgrImpl*) mgr.get();

	auto dbgClock = new DbgClock();
	dbgClock->pushTime("7:00");

	auto dbgMessageProvider = new DbgMessageProvider();
	dbgMessageProvider->pushMessage(1, "0649882351",
			"unsubscribe 8:00 - 10:00");

	auto dbgConfigProvider = new DbgConfigProvider();
	dbgConfigProvider->ga_ini = "angel_0_interval_0_end=10:00:00\n"
			"angel_0_interval_0_start= 8:00:00\n"
			"angel_0_nrIntervals=1\n"
			"angel_0_phoneNr=0649882351\n"
			"nr_angels=1\n";
	dbgConfigProvider->loadProperties();

	auto dbgActivityDetector = new DbgActivityDetector();

	mgr->setTimeProvider(std::shared_ptr<ITimeProvider>(dbgClock));
	mgr->setConfigProvider(std::shared_ptr<IConfigProvider>(dbgConfigProvider));
	mgr->setMessageProvider(
			std::shared_ptr<IMessageProvider>(dbgMessageProvider));
	mgr->setActivityDetector(
			std::shared_ptr<IActivityDetector>(dbgActivityDetector));

	tickMgr->doTick();

//	std::cerr << dbgConfigProvider->ga_ini << std::endl;
	ASSERT_EQ(dbgConfigProvider->ga_ini, "angel_0_interval_0_end=10:00:00\n"
			"angel_0_interval_0_start=8:00:00\n"
			"angel_0_nrIntervals=1\n"
			"angel_0_phoneNr=0649882351\n"
			"nr_angels=0\n");
}

TEST(AngelMgrSubscribe, UnSubscriptionAngel) {
	auto mgr = AngelMgrFactory::create();
	auto tickMgr = (AngelMgrImpl*) mgr.get();

	auto dbgClock = new DbgClock();
	dbgClock->pushTime("7:00");

	auto dbgMessageProvider = new DbgMessageProvider();
	dbgMessageProvider->pushMessage(1, "0649882351", "unsubscribe");

	auto dbgConfigProvider = new DbgConfigProvider();
	dbgConfigProvider->ga_ini = "angel_0_interval_0_end=10:00:00\n"
			"angel_0_interval_0_start= 8:00:00\n"
			"angel_0_nrIntervals=1\n"
			"angel_0_phoneNr=0649882351\n"
			"nr_angels=1\n";
	dbgConfigProvider->loadProperties();

	auto dbgActivityDetector = new DbgActivityDetector();

	mgr->setTimeProvider(std::shared_ptr<ITimeProvider>(dbgClock));
	mgr->setConfigProvider(std::shared_ptr<IConfigProvider>(dbgConfigProvider));
	mgr->setMessageProvider(
			std::shared_ptr<IMessageProvider>(dbgMessageProvider));
	mgr->setActivityDetector(
			std::shared_ptr<IActivityDetector>(dbgActivityDetector));

	tickMgr->doTick();

//	std::cerr << dbgConfigProvider->ga_ini << std::endl;
	ASSERT_EQ(dbgConfigProvider->ga_ini, "angel_0_interval_0_end=10:00:00\n"
			"angel_0_interval_0_start=8:00:00\n"
			"angel_0_nrIntervals=1\n"
			"angel_0_phoneNr=0649882351\n"
			"nr_angels=0\n");

}
