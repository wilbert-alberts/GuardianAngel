/*
 * MessageFactory.hpp
 *
 *  Created on: 5 okt. 2021
 *      Author: wilbert
 */

#ifndef LIB_GSM_MESSAGEFACTORY_HPP_
#define LIB_GSM_MESSAGEFACTORY_HPP_

#include <memory>
#include <string>

#include "IMessage.hpp"

namespace MessageFactory {
	std::shared_ptr<IMessage> createMessage(const MessageID id, const std::string sender, const std::string body);
}



#endif /* LIB_GSM_MESSAGEFACTORY_HPP_ */
