/*
 * TestTime24Interval.cpp
 *
 *  Created on: 26 sep. 2021
 *      Author: wilbert
 */

#include <memory>

#include "gtest/gtest.h"

#include "Time24.hpp"
#include "Time24Factory.hpp"
#include "WatchInterval.hpp"
#include "WatchIntervalFactory.hpp"


TEST(WatchIntervalImpl, InsideWorkingTimeInterval) {
	auto begin = Time24Factory::create(8,0,0);
	auto end = Time24Factory::create(16,30,0);
	auto lunch = Time24Factory::create(11,30,0);

	auto workingTime = WatchIntervalFactory::create(*begin, *end);

	ASSERT_TRUE(workingTime->insideInterval(*lunch));
}

TEST(WatchIntervalImpl, BeforeWorkingTimeInterval) {
	auto begin = Time24Factory::create(8,0,0);
	auto end = Time24Factory::create(16,30,0);
	auto wakeup = Time24Factory::create(7,0,0);

	auto workingTime = WatchIntervalFactory::create(*begin, *end);

	ASSERT_FALSE(workingTime->insideInterval(*wakeup));
}

TEST(WatchIntervalImpl, AfterWorkingTimeInterval) {
	auto begin = Time24Factory::create(8,0,0);
	auto end = Time24Factory::create(16,30,0);
	auto bedTime = Time24Factory::create(7,0,0);

	auto workingTime = WatchIntervalFactory::create(*begin, *end);

	ASSERT_FALSE(workingTime->insideInterval(*bedTime));
}


TEST(WatchIntervalImpl, InsidePartyTimeInterval) {
	auto begin = Time24Factory::create(20,0,0);
	auto end = Time24Factory::create(1,30,0);
	auto midnight= Time24Factory::create(0,0,0);

	auto partyTime = WatchIntervalFactory::create(*begin, *end);

	ASSERT_TRUE(partyTime->insideInterval(*midnight));
}

TEST(WatchIntervalImpl, TooEarlyToParty) {
	auto begin = Time24Factory::create(20,0,0);
	auto end = Time24Factory::create(1,30,0);
	auto noon= Time24Factory::create(18,0,0);

	auto partyTime = WatchIntervalFactory::create(*begin, *end);

	ASSERT_FALSE(partyTime->insideInterval(*noon));
}

TEST(WatchIntervalImpl, PartyIsOver) {
	auto begin = Time24Factory::create(20,0,0);
	auto end = Time24Factory::create(1,30,0);
	auto justPassed= Time24Factory::create(1,40,0);

	auto partyTime = WatchIntervalFactory::create(*begin, *end);

	ASSERT_FALSE(partyTime->insideInterval(*justPassed));
}



