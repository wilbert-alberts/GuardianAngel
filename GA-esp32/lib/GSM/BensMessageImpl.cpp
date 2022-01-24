/*
 * BensMessageImpl.cpp
 *
 *  Created on: 5 okt. 2021
 *      Author: wilbert
 */

#include "BensMessage.hpp"
#include "DateTime.hpp"

#include <regex>

class BensMessageImpl: public BensMessage {
public:
	BensMessageImpl (MessageID id, DateTime sentAt, const std::string &sender,	const std::string &body);
	virtual ~BensMessageImpl();

	virtual MessageID          getMessageID () const;
	virtual bool               isValid      () const;
    virtual const DateTime&    getSentAt    () const;
	virtual const std::string& getSender    () const;
	virtual const std::string& getBody      () const;
	virtual const std::string& getAction    () const; // Should be 'subscribe', 'unsubscribe'
	virtual const std::string& getStart     () const; // Should be in hh:mm or ''
	virtual const std::string& getEnd       () const; // Should be in hh:mm or ''

private:
	MessageID   messageID;
	bool        valid;
    DateTime    sentAt;
	std::string sender;
    std::string body;
	std::string action;
	std::string start;
	std::string end;

	static std::regex initBensMessageRx();
	static const std::regex messageRx;

	void parseBody(const std::string &body);
};

const std::regex BensMessageImpl::messageRx(BensMessageImpl::initBensMessageRx());


namespace BensMessageFactory {
std::shared_ptr<BensMessage> createBensMessage (MessageID id, DateTime sentAt, const std::string &sender, const std::string &body) {
	return std::shared_ptr<BensMessage>(new BensMessageImpl (id, sentAt, sender, body));
}

}

BensMessageImpl::BensMessageImpl (MessageID msgId, DateTime sentAt, const std::string &sender, const std::string &body) :
		messageID(msgId), valid(false), sentAt(sentAt), sender(sender), body (body), action("null"), start(""), end("") {
	parseBody(body);
}

BensMessageImpl::~BensMessageImpl() {
}

MessageID BensMessageImpl::getMessageID() const {
	return messageID;
}

bool BensMessageImpl::isValid() const {
	return valid;
}
const DateTime& BensMessageImpl::getSentAt() const {
	return sentAt;
}
const std::string& BensMessageImpl::getSender() const {
	return sender;
}
const std::string& BensMessageImpl::getBody() const {
	return body;
}

const std::string& BensMessageImpl::getAction() const {
	return action;
}

const std::string& BensMessageImpl::getStart() const {
	return start;
}

const std::string& BensMessageImpl::getEnd() const {
	return end;
}

std::regex BensMessageImpl::initBensMessageRx() {
	std::string timeRx = "[0-2]?[0-9]:[0-5][0-9]";
	std::string actionRx = "\\s*(subscribe|unsubscribe)\\s*";
	std::string startRx = "start\\s+(" + timeRx + ")";
	std::string endRx = "end\\s+(" + timeRx + ")";
	std::string intervalRx = "(" + timeRx + ")\\s*-\\s*(" + timeRx + ")";
	std::string timingRx = "((" + startRx + "\\s+" + endRx + ")|(" + intervalRx
			+ "))";
	std::string msgRx = "\\s*(" + actionRx + ")?(" + timingRx + ")?\\s*";

	return std::regex(msgRx);
}

void BensMessageImpl::parseBody(const std::string &body) {
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


