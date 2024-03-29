/*
 * Test24Test.cpp
 *
 *  Created on: 26 sep. 2021
 *      Author: wilbert
 */

#include <memory>

#include "gtest/gtest.h"

#include "Time24.hpp"
#include "Time24Factory.hpp"

TEST(Time24, CreateAndRetrieve) {
	auto t = Time24Factory::create(1, 2, 3);
	ASSERT_EQ(1, t->getHours());
	ASSERT_EQ(2, t->getMinutes());
	ASSERT_EQ(3, t->getSeconds());
}

TEST(Time24, RolloverSeconds) {
	auto t = Time24Factory::create(1, 2, 58);
	t->addSeconds(3);
	ASSERT_EQ(1, t->getHours());
	ASSERT_EQ(3, t->getMinutes());
	ASSERT_EQ(1, t->getSeconds());
}

TEST(Time24, RolloverMinutes) {
	auto t = Time24Factory::create(1, 58, 3);
	t->addMinutes(3);
	ASSERT_EQ(2, t->getHours());
	ASSERT_EQ(1, t->getMinutes());
	ASSERT_EQ(3, t->getSeconds());
}

TEST(Time24, RolloverHours) {
	auto t = Time24Factory::create(22, 2, 3);
	t->addHours(3);
	ASSERT_EQ(1, t->getHours());
	ASSERT_EQ(2, t->getMinutes());
	ASSERT_EQ(3, t->getSeconds());
}

TEST(Time24, PassMidnight) {
	auto t = Time24Factory::create(23, 59, 59);
	t->addSeconds(1);
	ASSERT_EQ(0, t->getHours());
	ASSERT_EQ(0, t->getMinutes());
	ASSERT_EQ(0, t->getSeconds());
}

TEST(Time24, TestEqual) {
	auto t1 = Time24Factory::create( 2,3,4);
	auto t2 = Time24Factory::create( 2,3,4);

	ASSERT_EQ(0, t1->compareTo(*t2));
}

TEST(Time24, TestEarlier) {
	auto t11 = Time24Factory::create( 1,3,4);
	auto t12 = Time24Factory::create( 2,1,4);
	auto t13 = Time24Factory::create( 2,3,1);
	auto t2 = Time24Factory::create( 2,3,4);

	ASSERT_LT(t11->compareTo(*t2),0);
	ASSERT_LT(t12->compareTo(*t2),0);
	ASSERT_LT(t13->compareTo(*t2),0);
}

TEST(Time24, TestLater) {
	auto t11 = Time24Factory::create( 2,3,5);
	auto t12 = Time24Factory::create( 2,4,4);
	auto t13 = Time24Factory::create( 3,3,4);
	auto t2 = Time24Factory::create( 2,3,4);

	ASSERT_GT(t11->compareTo(*t2),0);
	ASSERT_GT(t12->compareTo(*t2),0);
	ASSERT_GT(t13->compareTo(*t2),0);
}

TEST(Time24, TestString) {
	auto t1= Time24Factory::create(2,3,0);
	auto t11 = Time24Factory::create("2:03");
	auto t12 = Time24Factory::create("02:03");

	ASSERT_EQ(0, t1->compareTo(*t11));
	ASSERT_EQ(0, t1->compareTo(*t12));

	auto t2= Time24Factory::create(22,23,0);
	auto t21 = Time24Factory::create("22:23");

	ASSERT_EQ(0, t2->compareTo(*t21));
}

TEST(Time24, TestIllegalString) {
	auto t1 = Time24Factory::create("bogus");
	auto t2 = Time24Factory::create("24:00");
	auto t3 = Time24Factory::create("25:03");
	auto t4 = Time24Factory::create("25:60");
	auto t5 = Time24Factory::create("-25:00");
	auto t6 = Time24Factory::create("22:-2");

	ASSERT_EQ(nullptr, t1.get());
	ASSERT_EQ(nullptr, t2.get());
	ASSERT_EQ(nullptr, t3.get());
	ASSERT_EQ(nullptr, t4.get());
	ASSERT_EQ(nullptr, t5.get());
	ASSERT_EQ(nullptr, t6.get());
}
