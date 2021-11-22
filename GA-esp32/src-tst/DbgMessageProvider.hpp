/*
 * DbgMessageProvider.hpp
 *
 *  Created on: 18 nov. 2021
 *      Author: wilbert
 */

#ifndef SRC_TST_DBGMESSAGEPROVIDER_HPP_
#define SRC_TST_DBGMESSAGEPROVIDER_HPP_

#include <IMessage.hpp>
#include <IMessageProvider.hpp>
#include <memory>
#include <vector>


class DbgMessageProvider: public IMessageProvider {
public:
	DbgMessageProvider();

	void setMessages(std::vector<std::shared_ptr<IMessage>> messages);
	void pushMessage(MessageID id, const std::string& phoneNr, const std::string& content);

	virtual ~DbgMessageProvider() {
	}
	;
	virtual std::vector<MessageID> getMessageIDs() const;
	virtual std::shared_ptr<IMessage> getMessage(const MessageID id) const;
	virtual void delMessage(const MessageID id);
private:
	std::vector<std::shared_ptr<IMessage>> messages;

};


#endif /* SRC_TST_DBGMESSAGEPROVIDER_HPP_ */
