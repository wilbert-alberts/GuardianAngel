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
#include <PeriodicTask.hpp>
#include <platform.hpp>
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
	LOG("Starting.");

	doWiring();

	// Create active processes
	auto clockTask = timeProvider->createTask();
	clockTask->startTask();

	auto helpButtonTask = helpBtn->createTask();
	helpButtonTask->startTask();

	auto activityDetectorTask = activityDetector->createTask();
	activityDetectorTask->startTask();

	sleep(10);

	LOG("Stopping.");

	clockTask->endTask();
	delete clockTask;

	helpButtonTask->endTask();
	delete helpButtonTask;

	activityDetectorTask->endTask();
	delete activityDetectorTask;

	LOG("Stopped.");
	return 0;
}

#else


void setup()
{
	Serial.begin(115200);
	LOG("Starting.");

	doWiring();

	// Create active processes
	auto clockTask = timeProvider->createTask();
	clockTask->startTask();

	auto helpButtonTask = helpBtn->createTask();
	helpButtonTask->startTask();

	auto activityDetectorTask = activityDetector->createTask();
	activityDetectorTask->startTask();

	vTaskDelay(10 * 1000);

	LOG("Stopping.");

	clockTask->endTask();
	delete clockTask;

	helpButtonTask->endTask();
	delete helpButtonTask;

	activityDetectorTask->endTask();
	delete activityDetectorTask;
	
	LOG("Stopped.");
}

void loop()
{
}

#endif

void doWiring() {
	// Do all wiring
	LOG("Wiring.");
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

	LOG("Wiring finished.");

}

int readPIR() {
	return 0;
}

int readHelpBtn() {
	return 0;
}
