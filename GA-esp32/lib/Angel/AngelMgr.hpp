/*
 * AngelMgr.hpp
 *
 *  Created on: 5 okt. 2021
 *      Author: wilbert
 */

#ifndef LIB_ANGEL_ANGELMGR_HPP_
#define LIB_ANGEL_ANGELMGR_HPP_

#include <memory>


class WatchDog;
class ActivityDetector;
class GSM;
class Config;

class AngelMgr
{
public:
	virtual ~AngelMgr() {}

	virtual void setWatchDog(std::shared_ptr<WatchDog> wd) = 0;
	virtual void setHelpRequestDetector(std::shared_ptr<ActivityDetector> hr) = 0;
	virtual void setGSM(std::shared_ptr<GSM> gsm) = 0;
	virtual void setConfigProvider(std::shared_ptr<Config> cfg) = 0;

};



#endif /* LIB_ANGEL_ANGELMGR_HPP_ */
