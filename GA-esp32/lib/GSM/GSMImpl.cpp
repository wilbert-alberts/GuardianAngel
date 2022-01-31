/*
 * GSMImpl.cpp
 *
 *  Created on: 5 okt. 2021
 *      Author: wilbert
 */

#include <GSM.hpp>
#include <IMessage.hpp>
#include <MessageFactory.hpp>
#include <Time24.hpp>
#include <Time24Factory.hpp>
#include <algorithm>
#include <ctime>
#include <iterator>
#include <memory>
#include <string>
#include <vector>
#include "platform.hpp"

#ifdef GA_POSIX
#include <bits/types/struct_tm.h>
#include <chrono>
#include <time.h>

#else

#include <SIM800.h>
#include <BensSim.hpp>

#endif

class GSMImpl: public GSM {
public:
	GSMImpl();
	~GSMImpl() {
	}
	virtual std::vector<MessageID> getMessageIDs();
	virtual std::shared_ptr<IMessage> getMessage(const MessageID mid) const;
	virtual void delMessage(const MessageID mid);
	virtual void sendMessage(const std::string &phoneNr, const std::string& msg);
	virtual std::shared_ptr<Time24> getTime() const;

private:
	std::vector<std::shared_ptr<IMessage>> messages;
	static BensSim bs;
	static boolean bensSimBeginCalled;
};

// TODO: find way to configure PIN 
char bensPin[]="2881";
BensSim GSMImpl::bs (&SIM, bensPin);
boolean GSMImpl::bensSimBeginCalled(false);

namespace GSMFactory {
std::shared_ptr<GSM> create() {
	return std::shared_ptr<GSM>(new GSMImpl());
}
}

GSMImpl::GSMImpl() {
	LOG_ENTRY();
	if (!bensSimBeginCalled) {
		bs.begin();
		bensSimBeginCalled = true;
	}
	LOG_EXIT();
}

std::vector<MessageID> GSMImpl::getMessageIDs() {
	LOG_ENTRY();
	std::vector<MessageID> result;

	// Retrieve messages from BensSim and store in messages
	auto bensMessages = bs.getMessages();
	LOG("bensMessages retrieved");

	messages.clear();
	std::for_each(bensMessages.begin(), bensMessages.end(), [&](std::shared_ptr<BensMessage> bm){
		auto myMsg = MessageFactory::createMessage(bm->getMessageID(), bm->getSender(), bm->getBody());
		messages.push_back(myMsg);
	});
	LOG("bensMessages transformed in myMessages");

	// Extract message ids
	std::for_each(messages.begin(), messages.end(),
			[&](std::shared_ptr<IMessage> m) {
				result.push_back(m->getMessageID());
			});
	LOG("message ids extracted");
	LOG_EXIT();
	return result;

}

std::shared_ptr<IMessage> GSMImpl::getMessage(const MessageID mid) const {
	auto msg = std::find_if(messages.begin(), messages.end(),
			[&](std::shared_ptr<IMessage> m) {
				return m->getMessageID() == mid;
			});

	if (msg != messages.end()) {
		return *msg;
	} else {
		return std::shared_ptr<IMessage>(nullptr);
	}
}

void GSMImpl::delMessage(const MessageID mid) {
	auto newEnd = std::remove_if(messages.begin(), messages.end(),
			[&](std::shared_ptr<IMessage> m) {
				return m->getMessageID() == mid;
			});

	messages.erase(newEnd, messages.end());
}

void GSMImpl::sendMessage(const std::string &phoneNr, const std::string& msg) {
}

std::shared_ptr<Time24> GSMImpl::getTime() const {
	LOG_ENTRY();

	int h = 1;
	int m = 2;
	int s = 3;

#ifdef GA_POSIX
	auto n = std::chrono::system_clock::now();
	auto tt = std::chrono::system_clock::to_time_t(n);
	auto lt = localtime(&tt);

	h = lt->tm_hour;
	m = lt->tm_min;
	s = lt->tm_sec;

#else
	bs.getTime(h,m,s);
#endif

	std::shared_ptr<Time24> result = Time24Factory::create(h, m, s);
	LOG_EXIT("time: %s", result->toString().c_str());
	return result;

}
