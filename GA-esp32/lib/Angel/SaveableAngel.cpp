/*
 * SaveableAngel.cpp
 *
 *  Created on: 6 nov. 2021
 *      Author: wilbert
 */

#include <Config.hpp>
#include <Angel.hpp>
#include <SaveableAngel.hpp>
#include <Time24.hpp>
#include <Time24Interval.hpp>

#include <sstream>

SaveableAngel::SaveableAngel(std::shared_ptr<Angel> a, std::shared_ptr<Config> _cfg, int idx) : index(idx), angel(a), cfg(_cfg){

}

SaveableAngel::~SaveableAngel() {
	// TODO Auto-generated destructor stub
}

void SaveableAngel::save()
{
	savePhoneNr();
	saveIntervals();
}


void SaveableAngel::saveStr(const std::string& key, const std::string& value) {
	std::stringstream ss;
	ss << "angel_" << index << "_" << key;
	cfg->putProperty(ss.str(), value);
}

void SaveableAngel::savePhoneNr() {
	saveStr("phoneNr", angel->getPhoneNr());
}

void SaveableAngel::saveIntervals() {
	saveStr("nrIntervals", std::to_string(angel->getNrIntervals()));
	for (auto idx=0; idx<angel->getNrIntervals(); idx++) {
		saveInterval(idx, angel->getInterval(idx));
	}
}

void SaveableAngel::saveInterval(int idx, std::shared_ptr<Time24Interval> iv) {
	std::stringstream startKeyStream;
	startKeyStream << "interval_" << idx << "_" << "start";
	saveStr(startKeyStream.str(),  iv->getStart().toString());

	std::stringstream endKeyStream;
	endKeyStream << "interval_" << idx << "_" << "end";
	saveStr(endKeyStream.str(),  iv->getEnd().toString());
}
