/*
 * GSMImpl.cpp
 *
 *  Created on: 5 okt. 2021
 *      Author: wilbert
 */

#include <memory>
#include <vector>
#include <algorithm>

#include "GSM.hpp"
#include "GSMFactory.hpp"

class GSMImpl: public GSM {
public:
	GSMImpl();
	~GSMImpl() {
	}
	virtual std::vector<MessageID> getMessageIDs() const;
	virtual std::shared_ptr<Message> getMessage(const MessageID mid) const;
	virtual void delMessage(const MessageID mid);
private:
	std::vector<std::shared_ptr<Message>> messages;
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
			[](std::shared_ptr<Message> m) {
				return m->getMessageID();
			});
	return result;
}

std::shared_ptr<Message> GSMImpl::getMessage(const MessageID mid) const {
	auto msg = std::find_if(messages.begin(), messages.end(),
			[&](std::shared_ptr<Message> m) {
				return m->getMessageID() == mid;
			});

	if (msg != messages.end()) {
		return *msg;
	} else {
		return std::shared_ptr<Message>(nullptr);
	}
}

void GSMImpl::delMessage(const MessageID mid) {
	auto newEnd = std::remove_if(messages.begin(), messages.end(),
			[&](std::shared_ptr<Message> m) {
				return m->getMessageID() == mid;
			});

	messages.erase(newEnd, messages.end());
}

