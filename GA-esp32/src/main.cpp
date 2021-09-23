/*
 * main.cpp
 *
 *  Created on: 14 sep. 2021
 *      Author: wilbert
 */


#include <Arduino.h>

#include <iostream>

#include "ClockFactory.hpp"

void setup() {
	Serial.begin(115200);
	Serial.println("Started");
	
	auto clock = ClockFactory::create();

	// auto pt = new PeriodicTask("main", 1000);

}

void loop() {
}
