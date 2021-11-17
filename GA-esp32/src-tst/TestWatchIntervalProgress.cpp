/*
 * TestWatchIntervalProgress.cpp
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

TEST(TestWatchIntervalProgress, NormalProgressNoHelpNeeded) {
	auto begin = Time24Factory::create(8,0,0);
	auto end = Time24Factory::create(16,30,0);

	auto t1 = Time24Factory::create(7,0,0);
	auto t2 = Time24Factory::create(9,0,0);
	auto t3 = Time24Factory::create(10,0,0);
	auto t4 = Time24Factory::create(17,0,0);

	auto workingTime = WatchIntervalFactory::create(begin, end);

	ASSERT_EQ(IntervalState::PASSIVE, workingTime->getState());

	workingTime->timeProgress(t1);
	workingTime->activityDetected();
	ASSERT_EQ(IntervalState::PASSIVE, workingTime->getState());

	workingTime->timeProgress(t2);
	ASSERT_EQ(IntervalState::ACTIVE, workingTime->getState());

	workingTime->timeProgress(t3);
	workingTime->activityDetected();
	ASSERT_EQ(IntervalState::ACTIVE, workingTime->getState());

	workingTime->timeProgress(t4);
	ASSERT_EQ(IntervalState::PASSIVE, workingTime->getState());
}

TEST(TestWatchIntervalProgress, NormalProgressHelpNeeded) {
	auto begin = Time24Factory::create(8,0,0);
	auto end = Time24Factory::create(16,30,0);

	auto t1 = Time24Factory::create(7,0,0);
	auto t2 = Time24Factory::create(9,0,0);
	auto t3 = Time24Factory::create(10,0,0);
	auto t4 = Time24Factory::create(17,0,0);

	auto workingTime = WatchIntervalFactory::create(begin, end);

	ASSERT_EQ(IntervalState::PASSIVE, workingTime->getState());

	workingTime->timeProgress(t1);
	ASSERT_EQ(IntervalState::PASSIVE, workingTime->getState());

	workingTime->timeProgress(t2);
	ASSERT_EQ(IntervalState::ACTIVE, workingTime->getState());

	workingTime->timeProgress(t3);
	ASSERT_EQ(IntervalState::ACTIVE, workingTime->getState());

	workingTime->timeProgress(t4);
	ASSERT_EQ(IntervalState::HELPNEEDED, workingTime->getState());
}

TEST(TestWatchIntervalProgress, ActivityOutsideInterval) {
	auto begin = Time24Factory::create(8,0,0);
	auto end = Time24Factory::create(16,30,0);

	auto t1 = Time24Factory::create(7,0,0);
	auto t2 = Time24Factory::create(9,0,0);
	auto t3 = Time24Factory::create(10,0,0);
	auto t4 = Time24Factory::create(17,0,0);

	auto workingTime = WatchIntervalFactory::create(begin, end);

	ASSERT_EQ(IntervalState::PASSIVE, workingTime->getState());

	workingTime->timeProgress(t1);
	workingTime->activityDetected();
	ASSERT_EQ(IntervalState::PASSIVE, workingTime->getState());

	workingTime->timeProgress(t2);
	ASSERT_EQ(IntervalState::ACTIVE, workingTime->getState());

	workingTime->timeProgress(t3);
	ASSERT_EQ(IntervalState::ACTIVE, workingTime->getState());

	workingTime->timeProgress(t4);
	workingTime->activityDetected();
	ASSERT_EQ(IntervalState::HELPNEEDED, workingTime->getState());
}

TEST(TestWatchIntervalProgress, NormalProgressHelpNeededReset) {
	auto begin = Time24Factory::create(8,0,0);
	auto end = Time24Factory::create(16,30,0);

	auto t1 = Time24Factory::create(7,0,0);
	auto t2 = Time24Factory::create(9,0,0);
	auto t4 = Time24Factory::create(17,0,0);

	auto workingTime = WatchIntervalFactory::create(begin, end);

	ASSERT_EQ(IntervalState::PASSIVE, workingTime->getState());

	workingTime->timeProgress(t1);
	ASSERT_EQ(IntervalState::PASSIVE, workingTime->getState());

	workingTime->timeProgress(t2);
	ASSERT_EQ(IntervalState::ACTIVE, workingTime->getState());

	workingTime->timeProgress(t4);
	ASSERT_EQ(IntervalState::HELPNEEDED, workingTime->getState());

	workingTime->reset();
	ASSERT_EQ(IntervalState::PASSIVE, workingTime->getState());

}
