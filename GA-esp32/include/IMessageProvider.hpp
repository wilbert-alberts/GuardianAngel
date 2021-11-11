/*
 * IMessageProvider.hpp
 *
 *  Created on: 8 nov. 2021
 *      Author: wilbert
 */

#ifndef INCLUDE_IMESSAGEPROVIDER_HPP_
#define INCLUDE_IMESSAGEPROVIDER_HPP_

#include "IMessage.hpp"

#include <memory>
#include <vector>

class IMessageProvider {
public:
	virtual ~IMessageProvider(){};
	virtual std::vector<MessageID> getMessageIDs() const = 0;
	virtual std::shared_ptr<IMessage > getMessage(const MessageID id) const = 0;
	virtual void delMessage(const MessageID id) = 0;

};



#endif /* INCLUDE_IMESSAGEPROVIDER_HPP_ */
