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
public:

	ConfigImpl();
	virtual ~ConfigImpl();

	virtual void loadProperties(std::string& props);
	virtual void saveProperties(const std::string& props);
};


#endif /* LIB_CONFIG_CONFIGIMPL_HPP_ */
