/*
 * TestAngel.cpp
 *
 *  Created on: 16 nov. 2021
 *      Author: wilbert
 *
 */

#include "gtest/gtest.h"

#include "Angel.hpp"
#include "AngelFactory.hpp"
#include "WatchInterval.hpp"
#include "Time24.hpp"
#include "Time24Factory.hpp"

TEST(Angel, CreatePhoneNrCheck) {
	const std::string phoneNr = "+31612345678";
	auto angel = AngelFactory::create(phoneNr);

	ASSERT_EQ(phoneNr, angel->getPhoneNr());
}

TEST(Angel, AddIntervalCheck) {
	const std::string phoneNr = "+31612345678";
	auto angel = AngelFactory::create(phoneNr);

	angel->addInterval("8:00", "17:00");
	ASSERT_EQ(1, angel->getNrIntervals());
	ASSERT_FALSE(angel->helpNeeded());

	angel->addInterval("22:00", "23:00");
	ASSERT_EQ(2, angel->getNrIntervals());
	ASSERT_FALSE(angel->helpNeeded());

	auto iv = angel->getInterval(0);
	ASSERT_EQ(0, iv->getStart()->compareTo(*Time24Factory::create("8:00")));
	ASSERT_EQ(0, iv->getEnd()->compareTo(*Time24Factory::create("17:00")));

	auto iv2 = angel->getInterval(1);
	ASSERT_EQ(0, iv2->getStart()->compareTo(*Time24Factory::create("22:00")));
	ASSERT_EQ(0, iv2->getEnd()->compareTo(*Time24Factory::create("23:00")));
}

TEST(Angel, DelIntervalCheck) {
	const std::string phoneNr = "+31612345678";
	auto angel = AngelFactory::create(phoneNr);

	angel->addInterval("8:00", "17:00");
	ASSERT_EQ(1, angel->getNrIntervals());

	angel->addInterval("22:00", "23:00");
	ASSERT_EQ(2, angel->getNrIntervals());

	angel->delInterval("22:00", "23:00");
	ASSERT_EQ(1, angel->getNrIntervals());

	auto iv = angel->getInterval(0);
	ASSERT_EQ(0, iv->getStart()->compareTo(*Time24Factory::create("8:00")));
	ASSERT_EQ(0, iv->getEnd()->compareTo(*Time24Factory::create("17:00")));

	angel->addInterval("22:00", "23:00");
	ASSERT_EQ(2, angel->getNrIntervals());

	angel->delInterval("8:00", "17:00");
	ASSERT_EQ(1, angel->getNrIntervals());

	auto iv2 = angel->getInterval(0);
	ASSERT_EQ(0, iv2->getStart()->compareTo(*Time24Factory::create("22:00")));
	ASSERT_EQ(0, iv2->getEnd()->compareTo(*Time24Factory::create("23:00")));
}

TEST(Angel, HelpNeeded) {
	const std::string phoneNr = "+31612345678";
	auto angel = AngelFactory::create(phoneNr);

	angel->addInterval("8:00", "17:00");
	ASSERT_EQ(1, angel->getNrIntervals());
	ASSERT_FALSE(angel->helpNeeded());

	angel->timeProgress(Time24Factory::create("7:00"));
	ASSERT_FALSE(angel->helpNeeded());
	angel->timeProgress(Time24Factory::create("9:00"));
	ASSERT_FALSE(angel->helpNeeded());
	angel->timeProgress(Time24Factory::create("19:00"));
	ASSERT_TRUE(angel->helpNeeded());
}

TEST(Angel, HelpNeeded2) {
	const std::string phoneNr = "+31612345678";
	auto angel = AngelFactory::create(phoneNr);

	angel->addInterval("8:00", "10:00");
	ASSERT_EQ(1, angel->getNrIntervals());
	ASSERT_FALSE(angel->helpNeeded());

	angel->addInterval("18:00", "19:00");
	ASSERT_EQ(2, angel->getNrIntervals());
	ASSERT_FALSE(angel->helpNeeded());

	angel->timeProgress(Time24Factory::create("7:00"));
	ASSERT_FALSE(angel->helpNeeded());
	angel->timeProgress(Time24Factory::create("9:00"));
	angel->activityDetected();
	ASSERT_FALSE(angel->helpNeeded());
	angel->timeProgress(Time24Factory::create("11:00"));
	ASSERT_FALSE(angel->helpNeeded());

	angel->timeProgress(Time24Factory::create("12:00"));
	ASSERT_FALSE(angel->helpNeeded());
	angel->timeProgress(Time24Factory::create("18:30"));
	ASSERT_FALSE(angel->helpNeeded());
	angel->timeProgress(Time24Factory::create("19:30"));
	angel->activityDetected();
	ASSERT_TRUE(angel->helpNeeded());

	angel->reset();
	ASSERT_FALSE(angel->helpNeeded());
}


