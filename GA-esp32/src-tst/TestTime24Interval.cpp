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
#include "Time24Interval.hpp"
#include "Time24IntervalFactory.hpp"


TEST(Time24Interval, InsideWorkingTimeInterval) {
	auto begin = Time24Factory::create(8,0,0);
	auto end = Time24Factory::create(16,30,0);
	auto lunch = Time24Factory::create(11,30,0);

	auto workingTime = Time24IntervalFactory::create(*begin, *end);

	ASSERT_TRUE(workingTime->insideInterval(*lunch));
}

TEST(Time24Interval, BeforeWorkingTimeInterval) {
	auto begin = Time24Factory::create(8,0,0);
	auto end = Time24Factory::create(16,30,0);
	auto wakeup = Time24Factory::create(7,0,0);

	auto workingTime = Time24IntervalFactory::create(*begin, *end);

	ASSERT_FALSE(workingTime->insideInterval(*wakeup));
}

TEST(Time24Interval, AfterWorkingTimeInterval) {
	auto begin = Time24Factory::create(8,0,0);
	auto end = Time24Factory::create(16,30,0);
	auto bedTime = Time24Factory::create(7,0,0);

	auto workingTime = Time24IntervalFactory::create(*begin, *end);

	ASSERT_FALSE(workingTime->insideInterval(*bedTime));
}


TEST(Time24Interval, InsidePartyTimeInterval) {
	auto begin = Time24Factory::create(20,0,0);
	auto end = Time24Factory::create(1,30,0);
	auto midnight= Time24Factory::create(0,0,0);

	auto partyTime = Time24IntervalFactory::create(*begin, *end);

	ASSERT_TRUE(partyTime->insideInterval(*midnight));
}

TEST(Time24Interval, TooEarlyToParty) {
	auto begin = Time24Factory::create(20,0,0);
	auto end = Time24Factory::create(1,30,0);
	auto noon= Time24Factory::create(18,0,0);

	auto partyTime = Time24IntervalFactory::create(*begin, *end);

	ASSERT_FALSE(partyTime->insideInterval(*noon));
}

TEST(Time24Interval, PartyIsOver) {
	auto begin = Time24Factory::create(20,0,0);
	auto end = Time24Factory::create(1,30,0);
	auto justPassed= Time24Factory::create(1,40,0);

	auto partyTime = Time24IntervalFactory::create(*begin, *end);

	ASSERT_FALSE(partyTime->insideInterval(*justPassed));
}



