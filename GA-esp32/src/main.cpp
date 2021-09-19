/*
 * main.cpp
 *
 *  Created on: 14 sep. 2021
 *      Author: wilbert
 */


#include <Arduino.h>

#include <iostream>

#include "Config.hpp"
#include "ConfigFactory.hpp"

void test();

void setup() {
	Serial.begin(115200);
	Serial.println("Started");
	test();
}

void loop() {
}

void test() {
    Serial.println("Started");

    Config* config = ConfigFactory::create();

	config->putProperty("name", "wilbert");
	config->putProperty("email", "wilbert.alberts@gmx.net");

	auto n = config->getProperty("name");
	auto m = config->getProperty("email");

	Serial.printf("Name: %s\n", n->c_str());
	Serial.printf("Mail: %s\n", m->c_str());

	config->putProperty("name", "marielle");

	Serial.printf("Name: %s\n", n->c_str());
	Serial.printf("Mail: %s\n", m->c_str());
}
