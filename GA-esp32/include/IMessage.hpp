/*
 * Message.hpp
 *
 *  Created on: 5 okt. 2021
 *      Author: wilbert
 */

#ifndef IMESSAGE_HPP_
#define IMESSAGE_HPP_

#include <string>

typedef int MessageID;

class IMessage
{
public:
	virtual ~IMessage(){}
	virtual MessageID getMessageID() const = 0;
	virtual bool isValid() const = 0;
	virtual const std::string& getSender() const = 0;
	virtual const std::string& getAction() const = 0;// Should be 'subscribe', 'unsubscribe'
	virtual const std::string& getStart() const = 0; // Should be in hh:mm or ''
	virtual const std::string& getEnd() const = 0;   // Should be in hh:mm or ''
};


#endif /* IMESSAGE_HPP_ */
