/*
 * IMessageSender.hpp
 *
 *  Created on: 8 nov. 2021
 *      Author: wilbert
 */

#ifndef INCLUDE_IMESSAGESENDER_HPP_
#define INCLUDE_IMESSAGESENDER_HPP_

#include <string>

class IMessageSender {
public:
	virtual ~IMessageSender() {};
	virtual void sendMessage(const std::string& phoneNr, const std::string& msg) = 0;
};



#endif /* INCLUDE_IMESSAGESENDER_HPP_ */
