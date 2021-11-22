/*
 * DbgClock.hpp
 *
 *  Created on: 18 nov. 2021
 *      Author: wilbert
 */

#ifndef SRC_TST_DBGCLOCK_HPP_
#define SRC_TST_DBGCLOCK_HPP_

#include <Clock.hpp>
#include <memory>
#include <string>
#include <vector>

class DbgClock: public Clock {
public:
	DbgClock() {}
	virtual ~DbgClock() {
	}
	;
	void setTimes(std::vector<std::string> times);
	void pushTime(std::string time);

	virtual std::shared_ptr<Time24> getTime() const;
	virtual void advanceTime();

	virtual void setGSM(std::shared_ptr<GSM> gsm);
private:
	std::vector<std::shared_ptr<Time24>> times;
};

#endif /* SRC_TST_DBGCLOCK_HPP_ */
