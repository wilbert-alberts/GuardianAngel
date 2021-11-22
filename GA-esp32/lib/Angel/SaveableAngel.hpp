/*
 * SaveableAngel.h
 *
 *  Created on: 6 nov. 2021
 *      Author: wilbert
 */

#ifndef LIB_ANGEL_SAVEABLEANGEL_HPP_
#define LIB_ANGEL_SAVEABLEANGEL_HPP_

#include <memory>
#include <string>

class IConfigProvider;

class Angel;
class WatchInterval;

class SaveableAngel {
public:
	SaveableAngel(std::shared_ptr<Angel> a, std::shared_ptr<IConfigProvider> cfg, int idx);
	void save();

	virtual ~SaveableAngel();
private:
	int index;
	std::shared_ptr<Angel> angel;
	std::shared_ptr<IConfigProvider> cfg;

	void savePhoneNr();
	void saveIntervals();
	void saveInterval(int idx, std::shared_ptr<WatchInterval> iv);

	void saveStr(const std::string& key, const std::string& value);

};

#endif /* LIB_ANGEL_SAVEABLEANGEL_HPP_ */
