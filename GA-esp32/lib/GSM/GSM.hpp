/*
 * GSM.hpp
 *
 *  Created on: 5 okt. 2021
 *      Author: wilbert
 */

#ifndef LIB_GSM_GSM_HPP_
#define LIB_GSM_GSM_HPP_

#include <vector>
#include <memory>

#include "Message.hpp"
class Time24;

class GSM {
public:
	virtual ~GSM() {}
	virtual std::vector<MessageID> getMessageIDs() const = 0;
	virtual std::shared_ptr<Message> getMessage(const MessageID mid) const = 0;
	virtual void delMessage(const MessageID mid) = 0;
	virtual void sendMessage(const std::shared_ptr<std::string> phoneNr, const std::shared_ptr<std::string> msg) const = 0;
	virtual std::shared_ptr<Time24> getTime() const = 0;
};



#endif /* LIB_GSM_GSM_HPP_ */
