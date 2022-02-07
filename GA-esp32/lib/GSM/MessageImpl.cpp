/*
 * MessageImpl.cpp
 *
 *  Created on: 5 okt. 2021
 *      Author: wilbert
 */

#include <platform.hpp>
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
	LOG_ENTRY();
	LOG_EXIT("msgId: %d", messageID);
	return messageID;
}

bool MessageImpl::isValid() const {
	LOG_ENTRY();
	LOG_EXIT("valid: %s", boolToString(valid));
	return valid;
}
const std::string& MessageImpl::getSender() const {
	LOG_ENTRY();
	LOG_EXIT("sender: %s", sender.c_str());
	return sender;
}

const std::string& MessageImpl::getAction() const {
	LOG_ENTRY();
	LOG_EXIT("action: %s", action.c_str());
	return action;
}

const std::string& MessageImpl::getStart() const {
	LOG_ENTRY();
	LOG_EXIT("start: %s", start.c_str());
	return start;
}

const std::string& MessageImpl::getEnd() const {
	LOG_ENTRY();
	LOG_EXIT("end: %s", end.c_str());
	return end;
}

std::regex MessageImpl::initMessageRx() {
	LOG_ENTRY();
	std::string timeRx = "[0-2]?[0-9]:[0-5][0-9]";
	std::string actionRx = "\\s*(subscribe|unsubscribe)\\s*";
	std::string startRx = "start\\s+(" + timeRx + ")";
	std::string endRx = "end\\s+(" + timeRx + ")";
	std::string intervalRx = "(" + timeRx + ")\\s*-\\s*(" + timeRx + ")";
	std::string timingRx = "((" + startRx + "\\s+" + endRx + ")|(" + intervalRx
			+ "))";
	std::string msgRx = "\\s*(" + actionRx + ")?(" + timingRx + ")?\\s*";

	LOG_EXIT("msg regex: %s: ", msgRx.c_str());
	return std::regex(msgRx);
}

void MessageImpl::parseBody(const std::string &body) {
	LOG_ENTRY("body: %s" , body.c_str());
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
	LOG_EXIT("start: %s, end: %s, action: %s", start.c_str(), end.c_str(), action.c_str());
}


