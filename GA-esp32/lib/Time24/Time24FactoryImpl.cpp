/*
 * Time24Impl.cpp
 *
 *  Created on: 19 sep. 2021
 *      Author: wilbert
 */

#include <DbgTime24Impl.hpp>
#include <memory>
#include <string>
#include <platform.hpp>
#include <Time24Impl.hpp>

#ifdef TIME24_NORMAL

namespace Time24Factory {
std::shared_ptr<Time24> create(int h, int m, int s) {
	return std::shared_ptr<Time24>(new Time24Impl(h, m, s));
}

std::shared_ptr<Time24> create(std::string hhmm) {
	std::string hoursStr = hhmm.substr(0, hhmm.find(":"));
	std::string minsStr = hhmm.substr(hhmm.find(":") + 1);
	try {
		int hours = atoi(hoursStr.c_str());
		int mins = atoi(minsStr.c_str());

		if (hours >= 0 && hours < 24 && mins >= 0 && mins < 60)
			return create(hours, mins, 0);
		else
			return std::shared_ptr<Time24>(nullptr);
	} catch (...) {
		return std::shared_ptr<Time24>(nullptr);
	}
}
}

#endif

#ifdef TIME24_DEBUG

namespace Time24Factory {
std::shared_ptr<Time24> create(int h, int m, int s) {
	return std::shared_ptr<Time24>(new DbgTime24Impl(h, m, s));
}

std::shared_ptr<Time24> create(std::string hhmm) {
	std::string hoursStr = hhmm.substr(0, hhmm.find(":"));
	std::string minsStr = hhmm.substr(hhmm.find(":") + 1);
	try {
		int hours = stoi(hoursStr);
		int mins = stoi(minsStr);

		if (hours >= 0 && hours < 24 && mins >= 0 && mins < 60)
			return create(hours, mins, 0);
		else
			return std::shared_ptr<Time24>(nullptr);
	} catch (...) {
		return std::shared_ptr<Time24>(nullptr);
	}
}
}

#endif
