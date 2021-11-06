/*
 * main.cpp
 *
 *  Created on: 14 sep. 2021
 *      Author: wilbert
 */


#include <iostream>

#include "platform.hpp"

#include "ClockFactory.hpp"
#include "Clock.hpp"
#include "ActivityDetectorFactory.hpp"
#include "ConfigFactory.hpp"
#include "WatchDogFactory.hpp"
#include "WatchDog.hpp"
#include "AngelMgrFactory.hpp"
#include "AngelMgr.hpp"
#include "GSMFactory.hpp"

#ifdef GA_POSIX

#include <unistd.h>

int readPIR() {
	return 0;
}

int readHelpBtn() {
	return 0;
}

int main()
{
	std::cout << "Starting." << std::endl;

	auto configProvider = ConfigFactory::create();
	auto clock = ClockFactory::create();
	auto activityDetector = ActivityDetectorFactory::create(readPIR);
	auto helpBtn = ActivityDetectorFactory::create(readHelpBtn);
	auto gsm = GSMFactory::create();

	clock->setGSM(gsm);

	auto watchdog = WatchDogFactory::create();
	watchdog->setClock(clock);

	auto angelMgr = AngelMgrFactory::create();
	angelMgr->setWatchDog(watchdog);
	angelMgr->setHelpRequestDetector(helpBtn);

	configProvider->loadProperties();

	angelMgr->setConfigProvider(configProvider);

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
