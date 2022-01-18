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
    virtual void clear()=0;
    virtual void loadProperties()=0;
    virtual void saveProperties()=0;

    virtual void putProperty(const std::string& id, const std::string& value)=0;

    virtual const std::string* getProperty(const std::string& id)=0;

    virtual void loadProperties(std::string& props)=0;
    virtual void saveProperties(const std::string& props)=0;

};


#endif /* CONFIG_HPP_ */
