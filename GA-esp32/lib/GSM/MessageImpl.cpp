/*
 * MessageImpl.cpp
 *
 *  Created on: 5 okt. 2021
 *      Author: wilbert
 */

#include "IMessage.hpp"

#include <regex>

class MessageImpl: public IMessage {
public:
	MessageImpl(const MessageID msgId, const std::string &sender,
			const std::string &body);
	virtual ~MessageImpl();

	virtual MessageID getMessageID() const;
	virtual const std::string& getSender() const;
	virtual bool isValid() const;
	virtual const std::string& getAction() const; // Should be 'subscribe', 'unsubscribe'
	virtual const std::string& getStart() const; // Should be in hh:mm or ''
	virtual const std::string& getEnd() const;   // Should be in hh:mm or ''

private:
	MessageID messageID;
	bool valid;
	std::string sender;
	std::string action;
	std::string start;
	std::string end;

	static const std::regex messageRx;
	static std::regex initMessageRx();

	void parseBody(const std::string &body);
};

const std::regex MessageImpl::messageRx(MessageImpl::initMessageRx());

namespace MessageFactory {
std::shared_ptr<IMessage> createMessage(const MessageID id, const std::string sender, const std::string body) {
	return std::shared_ptr<IMessage>(new MessageImpl(id, sender, body));
}

}

MessageImpl::MessageImpl(const int msgId, const std::string &sender,
		const std::string &body) :
		messageID(msgId), valid(false), sender(sender), action("null"), start(""), end("") {
	parseBody(body);
}

MessageImpl::~MessageImpl() {
}

MessageID MessageImpl::getMessageID() const {
	return messageID;
}

bool MessageImpl::isValid() const {
	return valid;
}
const std::string& MessageImpl::getSender() const {
	return sender;
}

const std::string& MessageImpl::getAction() const {
	return action;
}

const std::string& MessageImpl::getStart() const {
	return start;
}

const std::string& MessageImpl::getEnd() const {
	return end;
}

std::regex MessageImpl::initMessageRx() {
	std::string timeRx = "[0-2]?[0-9]:[0-5][0-9]";
	std::string actionRx = "\\s*(subscribe|unsubscribe)\\s*";
	std::string startRx = "start\\s+(" + timeRx + ")";
	std::string endRx = "end\\s+(" + timeRx + ")";
	std::string intervalRx = "(" + timeRx + ")\\s*-\\s*(" + timeRx + ")";
	std::string timingRx = "((" + startRx + "\\s+" + endRx + ")|(" + intervalRx
			+ "))";
	std::string msgRx = "(" + actionRx + ")?(" + timingRx + ")?";
	return std::regex(msgRx);
}

void MessageImpl::parseBody(const std::string &body) {
	auto m = std::smatch { };
	bool regexMatch = std::regex_match(body, m, messageRx);

	if (regexMatch) {
		if (m[6] != "") {
			start = m[6];
		} else if (m[9] != "") {
			start = m[9];
		}
		if (m[7] != "") {
			end = m[7];
		} else if (m[10] != "") {
			end = m[10];
		}
		if (m[2] == "" || m[2] == "subscribe") {
			action = "subscribe";
			valid = true;
		}
		if (m[2] == "unsubscribe") {
			action = "unsubscribe";
			valid = true;
		}
	}
}


