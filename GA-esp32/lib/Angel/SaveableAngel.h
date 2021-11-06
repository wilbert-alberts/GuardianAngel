/*
 * SaveableAngel.h
 *
 *  Created on: 6 nov. 2021
 *      Author: wilbert
 */

#ifndef LIB_ANGEL_SAVEABLEANGEL_H_
#define LIB_ANGEL_SAVEABLEANGEL_H_

#include <memory>
#include <string>

class Angel;
class Config;
class Time24Interval;

class SaveableAngel {
public:
	SaveableAngel(std::shared_ptr<Angel> a, std::shared_ptr<Config> cfg, int idx);
	void save();

	virtual ~SaveableAngel();
private:
	int index;
	std::shared_ptr<Angel> angel;
	std::shared_ptr<Config> cfg;

	void savePhoneNr();
	void saveIntervals();
	void saveInterval(int idx, std::shared_ptr<Time24Interval> iv);

	void saveStr(const std::string& key, const std::string& value);

};

#endif /* LIB_ANGEL_SAVEABLEANGEL_H_ */
