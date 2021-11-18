/*
 * DbgMessageProvider.cpp
 *
 *  Created on: 18 nov. 2021
 *      Author: wilbert
 */

#include <IMessage.hpp>
#include <IMessageProvider.hpp>
#include <memory>
#include <vector>

class DbgMessageProvider: public IMessageProvider {
public:
	DbgMessageProvider();
	virtual ~DbgMessageProvider(){};
	virtual std::vector<MessageID> getMessageIDs() const = 0;
	virtual std::shared_ptr<IMessage > getMessage(const MessageID id) const = 0;
	virtual void delMessage(const MessageID id) = 0;

};


