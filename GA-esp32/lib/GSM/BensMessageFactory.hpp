/*
 * BensMessageFactory.hpp
 *
 *  Created on: 5 okt. 2021
 *      Author: wilbert
 *  Modified 19 Dec 2021 Ben
 */

#ifndef LIB_GSM_BENS_MESSAGEFACTORY_HPP_
#define LIB_GSM_BENS_MESSAGEFACTORY_HPP_

#include <memory>
#include <string>

#include "BensMessage.hpp"
#include "DateTime.hpp"

namespace BensMessageFactory {
	std::shared_ptr<BensMessage> createBensMessage(MessageID id, DateTime sentAt, const std::string &sender, const std::string &body);
}



#endif /* LIB_GSM_BENS_MESSAGEFACTORY_HPP_ */
