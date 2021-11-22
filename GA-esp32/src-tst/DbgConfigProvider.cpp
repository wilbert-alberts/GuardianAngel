/*
 * DbgConfigProvider.cpp
 *
 *  Created on: 18 nov. 2021
 *      Author: wilbert
 */

#include "DbgConfigProvider.hpp"

#include <sstream>
#include <vector>


void DbgConfigProvider::loadProperties() {
    auto result = std::vector<std::string>{};
    auto ss = std::stringstream{ga_ini};

    for (std::string line; std::getline(ss, line, '\n');) {
        result.push_back(line);
    }

	stringToProperties(result);
}

void DbgConfigProvider::saveProperties() {
	propertiesToString(ga_ini);
}
