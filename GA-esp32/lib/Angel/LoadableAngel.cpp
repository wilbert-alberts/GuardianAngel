/*
 * LoadableAngel.cpp
 *
 *  Created on: 6 nov. 2021
 *      Author: wilbert
 */

#include <LoadableAngel.h>

#include <Config.hpp>

#include <sstream>

LoadableAngel::LoadableAngel(std::shared_ptr<Config> _cfg, int _idx)
: index(_idx), cfg(_cfg)
{
}

LoadableAngel::~LoadableAngel() {
	// TODO Auto-generated destructor stub
}

const std::string& LoadableAngel::loadStr(const std::string& key) const {
	std::stringstream ss;

	ss << "angel_" << index << "_" << key;
	auto result = cfg->getProperty(ss.str());
	return *result;
}

const std::string& LoadableAngel::getPhoneNr() const {
	return loadStr("phoneNr");
}

int LoadableAngel::getNrIntervals() const {
	return std::stoi(loadStr("nrIntervals"));
}

const std::string& LoadableAngel::getStart(int intervalIdx) const {
	std::stringstream startKeyStream;
	startKeyStream << "interval_" << intervalIdx << "_" << "start";
	return loadStr(startKeyStream.str());

}

const std::string& LoadableAngel::getEnd(int intervalIdx) const {
	std::stringstream startKeyStream;
	startKeyStream << "interval_" << intervalIdx << "_" << "end";
	return loadStr(startKeyStream.str());
}
