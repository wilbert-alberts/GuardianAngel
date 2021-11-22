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
//
//TEST(AngelMgr, NewSubscription) {
//	auto mgr =  AngelMgrFactory::create();
//	auto tickMgr = (AngelMgrImpl*)mgr.get();
//
//
//	auto dbgClock = new DbgClock();
//	dbgClock->pushTime("7:00");
//
//	auto dbgMessageProvider = new DbgMessageProvider();
//	dbgMessageProvider->pushMessage(1, "0649882351", "subscribe 8:00 - 10:00");
//
//	auto dbgConfigProvider = new DbgConfigProvider();
//	dbgConfigProvider->ga_ini = "";
//
//	auto dbgActivityDetector = new DbgActivityDetector();
//
//	mgr->setTimeProvider(std::shared_ptr<ITimeProvider>(dbgClock));
//	mgr->setConfigProvider(std::shared_ptr<IConfigProvider>(dbgConfigProvider));
//	mgr->setMessageProvider(std::shared_ptr<IMessageProvider>(dbgMessageProvider));
//	mgr->setActivityDetector(std::shared_ptr<IActivityDetector>(dbgActivityDetector));
//
//
//	tickMgr->doTick();
//
//	std::cout << dbgConfigProvider->ga_ini << std::endl;
//
//
//}
