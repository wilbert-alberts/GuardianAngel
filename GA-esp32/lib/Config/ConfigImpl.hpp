/*
 * ConfigImpl.hpp
 *
 *  Created on: 18 nov. 2021
 *      Author: wilbert
 */

#ifndef LIB_CONFIG_CONFIGIMPL_HPP_
#define LIB_CONFIG_CONFIGIMPL_HPP_

#include <IConfigProvider.hpp>
#include <map>
#include <regex>
#include <string>
#include <vector>


class ConfigImpl: public IConfigProvider {
	const std::regex keyValueRx = std::regex("\\s*([^=]+)\\s*=\\s*(.*+)\\s*");

public:

	virtual ~ConfigImpl() {
	}
	virtual void clear();
	virtual void loadProperties();
	virtual void saveProperties();
	virtual void loadProperties(std::string& props);
	virtual void saveProperties(const std::string& props);

	virtual void putProperty(const std::string &id, const std::string &value);

	virtual const std::string* getProperty(const std::string &id);

protected:
	typedef std::map<std::string, std::string> PropertyMap;

	PropertyMap properties;

	void propertiesToString(std::string &str);
	void stringToProperties(const std::vector<std::string> &lines);

};


#endif /* LIB_CONFIG_CONFIGIMPL_HPP_ */
