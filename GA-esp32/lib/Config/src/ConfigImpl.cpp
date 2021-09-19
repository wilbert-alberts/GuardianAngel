/*
 * ConfigImpl.cpp
 *
 *  Created on: 14 sep. 2021
 *      Author: wilbert
 */

#include "Config.hpp"

#include <map>

class ConfigImpl : public Config
{
public:
	ConfigImpl() {} ;
	virtual ~ConfigImpl() {};
    virtual void clear();
    virtual void loadProperties() {};
    virtual void saveProperties() {};

    virtual void putProperty(const std::string &id, const std::string &value);
    virtual const std::string* getProperty(const std::string &id);

private:
    typedef std::map<std::string, std::string> PropertyMap;

    PropertyMap properties;
};

namespace ConfigFactory {

Config* create() {
	return new ConfigImpl();
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

const std::string* ConfigImpl::getProperty(const std::string &id) {
	auto entry = properties.find(id);
	if (entry == properties.end()) {
		return nullptr;
	} else {
		return &entry->second;
	}
}

