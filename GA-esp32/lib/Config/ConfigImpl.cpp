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
	std::ifstream of("GA.ini");

	std::vector<std::string> lines;
	std::string line;

	while (std::getline(of, line)) {
		lines.emplace_back(line);
	}

	stringToProperties(lines);

	of.close();
}

void ConfigImpl::saveProperties() {
	std::ofstream of("GA.ini");

	std::string output;
	propertiesToString(output);

	of << output;

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

void ConfigImpl::propertiesToString(std::string &str) {
	std::stringstream stream;

	std::for_each(properties.begin(), properties.end(),
			[&](PropertyMap::value_type p) {
				stream << p.first << "=" << p.second << std::endl;
			});
	str.assign(stream.str());
}

void ConfigImpl::stringToProperties(const std::vector<std::string> &lines) {
	std::for_each(lines.begin(), lines.end(), [&](std::string line) {
		std::smatch m;
		bool b = std::regex_match(line, m, keyValueRx);
		if (b) {
			std::string key = m[1];
			std::string value = m[2];
			properties.emplace(key, value);
		}
	});
}
