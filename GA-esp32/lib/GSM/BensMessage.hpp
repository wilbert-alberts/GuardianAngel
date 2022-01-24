/*
 * BensMessage.hpp
 *
 *  Created on: 5 okt. 2021
 *      Stolen from: wilbert
 *  Modified 19 dec 2021 Ben
 */

#ifndef LIB_GSM_BENS_MESSAGE_HPP_
#define LIB_GSM_BENS_MESSAGE_HPP_

#include <string>
#include "DateTime.hpp"

typedef int MessageID;

class BensMessage
{
public:
	virtual ~BensMessage                     (){}
	virtual MessageID          getMessageID  () const = 0;
	virtual bool               isValid       () const = 0;
    virtual const DateTime&    getSentAt     () const = 0;
	virtual const std::string& getSender     () const = 0;
    virtual const std::string& getBody       () const = 0;
	virtual const std::string& getAction     () const = 0;   // Should be 'subscribe', 'unsubscribe'
	virtual const std::string& getStart      () const = 0;   // Should be in hh:mm or ''
	virtual const std::string& getEnd        () const = 0;   // Should be in hh:mm or ''
};


#endif /* LIB_GSM_BENS_MESSAGE_HPP_ */
