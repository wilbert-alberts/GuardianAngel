/*
 * GSM.hpp
 *
 *  Created on: 5 okt. 2021
 *      Author: wilbert
 */

#ifndef LIB_GSM_GSM_HPP_
#define LIB_GSM_GSM_HPP_

#include <IMessage.hpp>
#include <IMessageProvider.hpp>
#include <IMessageSender.hpp>
#include <ITimeProvider.hpp>
#include <memory>
#include <string>
#include <vector>

class Time24;

class GSM: public IMessageSender, public IMessageProvider, public ITimeProvider {
public:
	virtual ~GSM() {}
};



#endif /* LIB_GSM_GSM_HPP_ */
