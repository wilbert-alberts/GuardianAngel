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

#include <platform.hpp>

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

#define CLEAR_EPROM

#ifdef CLEAR_EPROM
ConfigImpl::ConfigImpl() {
	LOG_ENTRY();
	EEPROM.begin(4096);
	EEPROM.writeString(0, "");
	LOG_EXIT();
}
#else
ConfigImpl::ConfigImpl() {
	EEPROM.begin(4096);
}
#endif

ConfigImpl::~ConfigImpl() {
	EEPROM.end();
}

void ConfigImpl::loadProperties(std::string& props) {
	LOG_ENTRY();
	String sprops = EEPROM.readString(0);
	props.clear();
	props = sprops.c_str();
	LOG("props: %s", props.c_str());
	LOG_EXIT();
}

void ConfigImpl::saveProperties(const std::string& props) {
	LOG_ENTRY();
	LOG("props: %s", props.c_str());
	EEPROM.writeString(0, props.c_str());
	LOG_EXIT();
}

#endif


