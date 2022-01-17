/*
 * GSMImpl.cpp
 *
 *  Created on: 5 okt. 2021
 *      Author: wilbert
 */

#include <GSM.hpp>
#include <IMessage.hpp>
#include <Time24.hpp>
#include <Time24Factory.hpp>
#include <algorithm>
#include <ctime>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

#ifdef GA_POSIX
#include <bits/types/struct_tm.h>
#include <chrono>
#include <time.h>

#endif

class GSMImpl: public GSM {
public:
	GSMImpl();
	~GSMImpl() {
	}
	virtual std::vector<MessageID> getMessageIDs() const;
	virtual std::shared_ptr<IMessage> getMessage(const MessageID mid) const;
	virtual void delMessage(const MessageID mid);
	virtual void sendMessage(const std::string &phoneNr, const std::string& msg);
	virtual std::shared_ptr<Time24> getTime() const;

private:
	std::vector<std::shared_ptr<IMessage>> messages;
};

namespace GSMFactory {
std::shared_ptr<GSM> create() {
	return std::shared_ptr<GSM>(new GSMImpl());
}
}

GSMImpl::GSMImpl() {
}

std::vector<MessageID> GSMImpl::getMessageIDs() const {

	std::vector<MessageID> result;

	std::transform(messages.begin(), messages.end(), result.begin(),
			[](std::shared_ptr<IMessage> m) {
				return m->getMessageID();
			});
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

#endif
	return Time24Factory::create(h, m, s);

}
