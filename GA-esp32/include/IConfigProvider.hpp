/*
 * Config.hpp
 *
 *  Created on: 14 sep. 2021
 *      Author: wilbert
 */

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <string>

class IConfigProvider
{
public:
	IConfigProvider() {};
	virtual ~IConfigProvider() {};

    virtual void loadProperties(std::string& props)=0;
    virtual void saveProperties(const std::string& props)=0;

};


#endif /* CONFIG_HPP_ */
