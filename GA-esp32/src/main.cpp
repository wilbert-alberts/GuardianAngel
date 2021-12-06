/*
 * main.cpp
 *
 *  Created on: 14 sep. 2021
 *      Author: wilbert
 */

#include <ActivityDetectorFactory.hpp>
#include <AlarmStation.hpp>
#include <AlarmStationFactory.hpp>
#include <AngelMgr.hpp>
#include <AngelMgrFactory.hpp>
#include <Clock.hpp>
#include <ClockFactory.hpp>
#include <ConfigFactory.hpp>
#include <GSM.hpp>
#include <GSMFactory.hpp>
#include <HelpButton.hpp>
#include <IActivityDetector.hpp>
#include <IButton.hpp>
#include <IConfigProvider.hpp>
#include <iostream>
#include <memory>

#ifdef GA_POSIX

#include <unistd.h>

int readPIR() {
	return 0;
}

int readHelpBtn() {
	return 0;
}

int main() {
	std::cout << "Starting." << std::endl;

	// Do all wiring
	auto configProvider = ConfigFactory::create();
	configProvider->loadProperties();

	auto gsm = GSMFactory::create();

	auto clock = ClockFactory::create();
	clock->setGSM(gsm);

	auto activityDetector = ActivityDetectorFactory::create(readPIR);

	auto helpBtn = HelpButtonFactory::create(readHelpBtn);

	auto alarmProcessor = AlarmStationFactory::create();
	alarmProcessor->setMessageSender(gsm);

	auto angelMgr = AngelMgrFactory::create();
	angelMgr->setConfigProvider(configProvider);
	angelMgr->setHelpButton(helpBtn);
	angelMgr->setMessageProvider(gsm);
	angelMgr->setActivityDetector(activityDetector);
	angelMgr->setAlarmProcessor(alarmProcessor);
	angelMgr->setTimeProvider(clock);

	// Create active processes
	auto helpButtonTask = helpBtn->createTask();
	auto activityDetectorTask = activityDetector->createTask();
	auto angelMgrTask = angelMgr->createTask();
	auto clockTask = clock->createTask();

	std::cout << "Started." << std::endl;

	sleep(10);

	std::cout << "Stopping." << std::endl;
	std::cout << "Stopped." << std::endl;

	return 0;
}

#else

void setup()
{
	Serial.begin(115200);
	Serial.println("Starting.");

	auto configProvider = ConfigFactory::create();
	auto clock = ClockFactory::create();
	auto activityDetector = ActivityDetectorFactory::create(readPIR);
	auto helpBtn = ActivityDetectorFactory::create(readHelpBtn);

	Serial.println("Started.");

	vTaskDelay(10 * 1000);

	Serial.println("Stopping.");
	Serial.println("Stopped.");
	
}

void loop()
{
}

#endif
