/*
 * main.cpp
 *
 *  Created on: 14 sep. 2021
 *      Author: wilbert
 */

#include <platform.hpp>

#include <iostream>

#include <ClockFactory.hpp>

#ifdef GA_POSIX

#include <unistd.h>

int main()
{
	std::cout << "Starting." << std::endl;

	auto clock = ClockFactory::create();
	std::cout << "Started." << std::endl;

	sleep(10);

	std::cout << "Stopping." << std::endl;
	delete (clock);
	std::cout << "Stopped." << std::endl;

	return 0;
}

#else

void setup()
{
	Serial.begin(115200);
	Serial.println("Starting.");

	auto clock = ClockFactory::create();
	Serial.println("Started.");

	vTaskDelay(10 * 1000);

	Serial.println("Stopping.");
	Serial.println("Stopped.");
	
}

void loop()
{
}

#endif
