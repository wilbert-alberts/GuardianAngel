/*
 * MessageTest.cpp
 *
 *  Created on: 5 okt. 2021
 *      Author: wilbert
 */

#include "gtest/gtest.h"

#include "MessageFactory.hpp"
#include "IMessage.hpp"

TEST(TestMessage, OnlySubscribe) {
	auto msg = MessageFactory::createMessage(1, "me", "subscribe");

	ASSERT_EQ(1, msg->getMessageID());
	ASSERT_TRUE(msg->isValid());
	ASSERT_EQ("me", msg->getSender());
	ASSERT_EQ("subscribe", msg->getAction());
	ASSERT_EQ("", msg->getStart());
	ASSERT_EQ("", msg->getEnd());
}

TEST(TestMessage, OnlyUnSubscribe) {
	auto msg = MessageFactory::createMessage(1, "me", "unsubscribe");

	ASSERT_EQ(1, msg->getMessageID());
	ASSERT_TRUE(msg->isValid());
	ASSERT_EQ("me", msg->getSender());
	ASSERT_EQ("unsubscribe", msg->getAction());
	ASSERT_EQ("", msg->getStart());
	ASSERT_EQ("", msg->getEnd());
}

TEST(TestMessage, Bogus) {
	auto msg = MessageFactory::createMessage(1, "me", "bogus");

	ASSERT_EQ(1, msg->getMessageID());
	ASSERT_FALSE(msg->isValid());
	ASSERT_EQ("me", msg->getSender());
}

TEST(TestMessage, SubscribeWithInterval) {
	auto msg = MessageFactory::createMessage(1, "me", "subscribe 7:00-8:00");

	ASSERT_EQ(1, msg->getMessageID());
	ASSERT_TRUE(msg->isValid());
	ASSERT_EQ("me", msg->getSender());
	ASSERT_EQ("subscribe", msg->getAction());
	ASSERT_EQ("7:00", msg->getStart());
	ASSERT_EQ("8:00", msg->getEnd());
}

TEST(TestMessage, SubscribeWithStartEnd) {
	auto msg = MessageFactory::createMessage(1, "me", "subscribe start  7:00 end 8:00");

	ASSERT_EQ(1, msg->getMessageID());
	ASSERT_TRUE(msg->isValid());
	ASSERT_EQ("me", msg->getSender());
	ASSERT_EQ("subscribe", msg->getAction());
	ASSERT_EQ("7:00", msg->getStart());
	ASSERT_EQ("8:00", msg->getEnd());
}

