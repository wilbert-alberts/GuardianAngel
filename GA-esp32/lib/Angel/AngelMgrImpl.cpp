/*
 * AngelMgrImpl.cpp
 *
 *  Created on: 5 okt. 2021
 *      Author: wilbert
 */

#include <Angel.hpp>
#include <AngelFactory.hpp>
#include <AngelMgr.hpp>
#include <GSM.hpp>
#include <Message.hpp>
#include <PeriodicTask.hpp>
#include <algorithm>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

class AngelMgrImpl: public AngelMgr, public PeriodicTask {
public:
	AngelMgrImpl();
	virtual ~AngelMgrImpl() {
	}

	virtual void setWatchDog(std::shared_ptr<WatchDog> wd);
	virtual void setHelpRequestDetector(std::shared_ptr<ActivityDetector> hr);
	virtual void setGSM(std::shared_ptr<GSM> gsm);

	virtual void tick();

	void processMessage(std::shared_ptr<Message> msg);
	void subscribeAngel(const std::string &phonenr, const std::string &start,
			const std::string &end);
	void unsubscribeAngel(const std::string &phonenr, const std::string &start,
			const std::string &end);

private:
	std::shared_ptr<WatchDog> wd;
	std::shared_ptr<ActivityDetector> hr;
	std::shared_ptr<GSM> gsm;

	std::vector<std::shared_ptr<Angel>> angels;
};

namespace AngelMgrFactory {
std::shared_ptr<AngelMgr> create() {
	return std::shared_ptr<AngelMgr>(new AngelMgrImpl());
}

}
AngelMgrImpl::AngelMgrImpl() :
		PeriodicTask("AngelMgr", 60 * 1000, 4000) {

}

void AngelMgrImpl::setWatchDog(std::shared_ptr<WatchDog> _wd) {
	wd = _wd;
}

void AngelMgrImpl::setHelpRequestDetector(
		std::shared_ptr<ActivityDetector> _hr) {
	hr = _hr;
}

void AngelMgrImpl::setGSM(std::shared_ptr<GSM> _gsm) {
	gsm = _gsm;
}

void AngelMgrImpl::tick() {
	auto msgs = gsm->getMessageIDs();
	std::for_each(msgs.begin(), msgs.end(), [&](MessageID mid) {
		auto msg = gsm->getMessage(mid);
		if (msg->isValid()) {
			processMessage(msg);
		}
	});
}

void AngelMgrImpl::processMessage(std::shared_ptr<Message> msg) {
	if (msg->getAction() == "subscribe") {
		subscribeAngel(msg->getSender(), msg->getStart(), msg->getEnd());
		gsm->delMessage(msg->getMessageID());
	}
	if (msg->getAction() == "unsubscribe") {
		unsubscribeAngel(msg->getSender(), msg->getStart(), msg->getEnd());
		gsm->delMessage(msg->getMessageID());
	}
}

void AngelMgrImpl::subscribeAngel(const std::string &phonenr,
		const std::string &start, const std::string &end) {
	auto angel = std::find_if(angels.begin(), angels.end(),
			[&](std::shared_ptr<Angel> a) {
				return a->getPhoneNr() == phonenr;
			});
	if (angel == angels.end()) {
		auto newAngel = AngelFactory::create(phonenr, wd, hr);
		angels.push_back(newAngel);
		angel = angels.end();
		angel--;
	}

	(*angel)->addInterval(start, end);
}

void AngelMgrImpl::unsubscribeAngel(const std::string &phonenr,
		const std::string &start, const std::string &end) {
	auto angel = std::find_if(angels.begin(), angels.end(),
			[&](std::shared_ptr<Angel> a) {
				return a->getPhoneNr() == phonenr;
			});
	if (angel != angels.end()) {
		if (start != "" || end != "")
			(*angel)->delInterval(start, end);
		else
			angels.erase(angel);
	}
}
