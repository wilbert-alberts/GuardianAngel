/*
 * LoadableAngel.h
 *
 *  Created on: 6 nov. 2021
 *      Author: wilbert
 */

#ifndef LIB_ANGEL_LOADABLEANGEL_H_
#define LIB_ANGEL_LOADABLEANGEL_H_

#include <memory>

class Config;


class LoadableAngel {
public:
	LoadableAngel(std::shared_ptr<Config> cfg, int idx);
	virtual ~LoadableAngel();

	const std::string& getPhoneNr() const;

	int getNrIntervals() const;
	const std::string& getStart(int intervalIdx) const;
	const std::string& getEnd(int intervalIdx) const;

private:
	int index;
	std::shared_ptr<Config> cfg;

	const std::string& loadStr(const std::string& key) const;
};

#endif /* LIB_ANGEL_LOADABLEANGEL_H_ */
