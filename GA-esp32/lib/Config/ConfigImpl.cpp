/*
 * ConfigImpl.cpp
 *
 *  Created on: 14 sep. 2021
 *      Author: wilbert
 */

#include <ConfigImpl.hpp>
#include <algorithm>
#include <memory>
#include <sstream>
#include <utility>


namespace ConfigFactory {

std::shared_ptr<IConfigProvider> create() {
	return std::shared_ptr<IConfigProvider>(new ConfigImpl());
}

}

#ifdef GA_POSIX

#include <fstream>

ConfigImpl::ConfigImpl() {
}

ConfigImpl::~ConfigImpl() {
}

void ConfigImpl::loadProperties(std::string& props) {
	std::ifstream of("GA.ini");

	of >> props;

	of.close();
}

void ConfigImpl::saveProperties(const std::string& props) {
	std::ofstream of("GA.ini");

	of << props;

	of.close();
}

#else

#include <EEPROM.h>

ConfigImpl::ConfigImpl() {
	EEPROM.begin(4096);
}

ConfigImpl::~ConfigImpl() {
	EEPROM.end();
}

void ConfigImpl::loadProperties(std::string& props) {
	String sprops = EEPROM.readString(0);
	props.clear();
	props = sprops.c_str();
}

void ConfigImpl::saveProperties(const std::string& props) {
	EEPROM.writeString(0, props.c_str());
}

#endif


