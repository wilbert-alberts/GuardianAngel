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

int readPIR();
int readHelpBtn();
void doWiring();

static std::shared_ptr<IConfigProvider> configProvider;
static std::shared_ptr<GSM> 			gsm;
static std::shared_ptr<Clock> 			timeProvider;
static std::shared_ptr<AlarmStation> 	alarmProcessor;
static std::shared_ptr<AngelMgr> 		angelMgr;

static std::shared_ptr<IButton> 		helpBtn;
static std::shared_ptr<IActivityDetector> activityDetector;

#ifdef GA_POSIX

#include <unistd.h>

int main() {
	std::cout << "Starting." << std::endl;

	doWiring();

	// Create active processes
	auto clockTask = timeProvider->createTask();
	auto helpButtonTask = helpBtn->createTask();
	auto activityDetectorTask = activityDetector->createTask();
	auto angelMgrTask = angelMgr->createTask();

	sleep(10);

	std::cout << "Stopping." << std::endl;
	std::cout << "Stopped." << std::endl;

	return 0;
}

#else

#include <Arduino.h>

void setup()
{
	Serial.begin(115200);
	Serial.println("Starting.");

	doWiring();

	vTaskDelay(10 * 1000);

	Serial.println("Stopping.");
	Serial.println("Stopped.");
	
}

void loop()
{
}

#endif

void doWiring() {
	// Do all wiring
	std::cout << "Wiring." << std::endl;
	configProvider = ConfigFactory::create();

	gsm = GSMFactory::create();

	timeProvider = ClockFactory::create();
	timeProvider->setGSM(gsm);

	activityDetector = ActivityDetectorFactory::create(readPIR);

	helpBtn = HelpButtonFactory::create(readHelpBtn);

	alarmProcessor = AlarmStationFactory::create();
	alarmProcessor->setMessageSender(gsm);

	angelMgr = AngelMgrFactory::create();
	angelMgr->setConfigProvider(configProvider);
	angelMgr->setHelpButton(helpBtn);
	angelMgr->setMessageProvider(gsm);
	angelMgr->setActivityDetector(activityDetector);
	angelMgr->setAlarmProcessor(alarmProcessor);
	angelMgr->setTimeProvider(timeProvider);

	std::cout << "Wiring finished." << std::endl;

}

int readPIR() {
	return 0;
}

int readHelpBtn() {
	return 0;
}
