/*
 * ConfigImpl.cpp
 *
 *  Created on: 14 sep. 2021
 *      Author: wilbert
 */

#include "Config.hpp"

#include <memory>
#include <map>
#include <sstream>
#include <algorithm>
#include <regex>

class ConfigImpl: public Config {
public:
	ConfigImpl() {
	}
	;
	virtual ~ConfigImpl() {
	}
	virtual void clear();
	virtual void loadProperties();
	virtual void saveProperties();

	virtual void putProperty(const std::string &id, const std::string &value);

	virtual const std::string* getProperty(const std::string &id);

private:
	typedef std::map<std::string, std::string> PropertyMap;

	PropertyMap properties;

//	const std::basic_regex keyValueRx("([^=]+)=(.+)");
};

namespace ConfigFactory {

std::shared_ptr<Config> create() {
	return std::shared_ptr<Config>(new ConfigImpl());
}

}

void ConfigImpl::clear() {
	properties.erase(properties.begin(), properties.end());
}

void ConfigImpl::putProperty(const std::string &id, const std::string &value) {
	auto entry = properties.find(id);
	if (entry == properties.end()) {
		properties.emplace(id, value);
	} else {
		entry->second = value;
	}
}

#ifdef GA_POSIX

#include <fstream>

void ConfigImpl::loadProperties() {
	std::ifstream of("GA.ini" );

	std::string line;
	while(std::getline(of, line)) {
		std::smatch m;

		auto b= std::regex_match(line.begin(), line.end(), m, "asdfasdf");
	}

	of.close();

}

void ConfigImpl::saveProperties() {
	std::ofstream of("GA.ini" );

	std::for_each(properties.begin(), properties.end(), [&](PropertyMap::value_type p){
		of << p.first << "=" <<p.second << std::endl;
	});

	of.close();
}

#else

void ConfigImpl::loadProperties() {
}

void ConfigImpl::saveProperties() {
}

#endif

const std::string* ConfigImpl::getProperty(const std::string &id) {
	auto entry = properties.find(id);
	if (entry == properties.end()) {
		return nullptr;
	} else {
		return &entry->second;
	}
}

