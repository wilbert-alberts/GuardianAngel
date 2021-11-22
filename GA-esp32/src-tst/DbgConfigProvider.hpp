/*
 * DbgConfigProvider.hpp
 *
 *  Created on: 18 nov. 2021
 *      Author: wilbert
 */

#ifndef SRC_TST_DBGCONFIGPROVIDER_HPP_
#define SRC_TST_DBGCONFIGPROVIDER_HPP_

#include <ConfigImpl.hpp>
#include <string>



class DbgConfigProvider: public ConfigImpl {
public:
	virtual void loadProperties();
	virtual void saveProperties();

	std::string ga_ini;
};


#endif /* SRC_TST_DBGCONFIGPROVIDER_HPP_ */
