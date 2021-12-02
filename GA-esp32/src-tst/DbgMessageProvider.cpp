/*
 * DbgMessageProvider.cpp
 *
 *  Created on: 18 nov. 2021
 *      Author: wilbert
 */

#include "DbgMessageProvider.hpp"

#include <MessageFactory.hpp>
#include <algorithm>
#include <iterator>
#include <string>

DbgMessageProvider::DbgMessageProvider() {

}

void DbgMessageProvider::setMessages(
		std::vector<std::shared_ptr<IMessage>> ms) {
	messages.clear();
	std::for_each(ms.begin(), ms.end(), [this](auto m) {
		messages.push_back(m);
	});
}

void DbgMessageProvider::pushMessage(MessageID id, const std::string& phoneNr, const std::string& content) {
	messages.push_back(MessageFactory::createMessage(id, phoneNr, content));
}

std::vector<MessageID> DbgMessageProvider::getMessageIDs() const {
	std::vector<MessageID> r;
	std::for_each(messages.begin(), messages.end(), [&](auto msg) {
				r.push_back(msg->getMessageID());
			});

	return r;
}

std::shared_ptr<IMessage> DbgMessageProvider::getMessage(const MessageID id) const
{
	auto r = std::find_if(messages.begin(), messages.end(), [id](auto msg) {
		return id == msg->getMessageID();
	});
	return *r;
}

void DbgMessageProvider::delMessage(const MessageID id) {
	auto newEnd = std::remove_if(messages.begin(), messages.end(), [id](auto msg) {
		return id == msg->getMessageID();
	});
	messages.erase(newEnd, messages.end());
}
