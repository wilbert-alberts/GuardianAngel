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

class GSM {
public:
	virtual ~GSM() {}
	virtual std::vector<MessageID> getMessageIDs() const = 0;
	virtual std::shared_ptr<Message> getMessage(const MessageID mid) const = 0;
	virtual void delMessage(const MessageID mid) = 0;
};



#endif /* LIB_GSM_GSM_HPP_ */
