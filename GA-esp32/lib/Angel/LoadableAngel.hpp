/*
 * LoadableAngel.h
 *
 *  Created on: 6 nov. 2021
 *      Author: wilbert
 */

#ifndef LIB_ANGEL_LOADABLEANGEL_HPP_
#define LIB_ANGEL_LOADABLEANGEL_HPP_

#include <memory>
#include <string>

class IConfigProvider;

class LoadableAngel {
public:
	LoadableAngel(std::shared_ptr<IConfigProvider> cfg, int idx);
	virtual ~LoadableAngel();

	const std::string& getPhoneNr() const;

	int getNrIntervals() const;
	const std::string& getStart(int intervalIdx) const;
	const std::string& getEnd(int intervalIdx) const;

	std::shared_ptr<Angel> toAngel();

private:
	int index;
	std::shared_ptr<IConfigProvider> cfg;

	const std::string& loadStr(const std::string& key) const;
};

#endif /* LIB_ANGEL_LOADABLEANGEL_HPP_ */
