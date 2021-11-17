/*
 * TestWatchIntervalGetters.cpp
 *
 *  Created on: 11 nov. 2021
 *      Author: wilbert
 */

#include <gtest/gtest.h>
#include <Time24.hpp>
#include <Time24Factory.hpp>
#include <WatchInterval.hpp>
#include <WatchIntervalFactory.hpp>
#include <memory>

TEST(TestWatchInterval, Getters) {
	auto begin = Time24Factory::create(8, 0, 0);
	auto end = Time24Factory::create(16, 30, 0);

	auto t1 = Time24Factory::create(8,0,0);
	auto t2 = Time24Factory::create(16,30,0);
	auto t3 = Time24Factory::create(10, 3, 4);
	auto t4 = Time24Factory::create(17, 5, 6);

	auto workingTime = WatchIntervalFactory::create(begin, end);

	ASSERT_EQ(0, workingTime->getStart()->compareTo(*t1));
	ASSERT_EQ(0, workingTime->getEnd()->compareTo(*t2));
	ASSERT_FALSE(0 == workingTime->getStart()->compareTo(*t3));
	ASSERT_FALSE(0 == workingTime->getEnd()->compareTo(*t4));
}

TEST(TestWatchInterval, Matches) {
	auto begin = Time24Factory::create(8, 0, 0);
	auto end = Time24Factory::create(16, 30, 0);

	auto t1 = Time24Factory::create(8,0,0);
	auto t2 = Time24Factory::create(16,30,0);
	auto t3 = Time24Factory::create(10, 3, 4);
	auto t4 = Time24Factory::create(17, 5, 6);

	auto workingTime = WatchIntervalFactory::create(begin, end);

	ASSERT_TRUE(workingTime->matches(begin, end));
	ASSERT_TRUE(workingTime->matches(t1, t2));
	ASSERT_FALSE(workingTime->matches(t3, t4));
	ASSERT_FALSE(workingTime->matches(begin, t3));
	ASSERT_FALSE(workingTime->matches(t4, end));

}

