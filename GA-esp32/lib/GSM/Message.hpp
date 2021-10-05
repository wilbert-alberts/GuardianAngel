/*
 * Message.hpp
 *
 *  Created on: 5 okt. 2021
 *      Author: wilbert
 */

#ifndef LIB_GSM_MESSAGE_HPP_
#define LIB_GSM_MESSAGE_HPP_

#include <string>

typedef int MessageID;

class Message
{
public:
	virtual ~Message(){}
	virtual MessageID getMessageID() const = 0;
	virtual bool isValid() const = 0;
	virtual const std::string& getSender() const = 0;
	virtual const std::string& getAction() const = 0;// Should be 'subscribe', 'unsubscribe'
	virtual const std::string& getStart() const = 0; // Should be in hh:mm or ''
	virtual const std::string& getEnd() const = 0;   // Should be in hh:mm or ''
};


#endif /* LIB_GSM_MESSAGE_HPP_ */
