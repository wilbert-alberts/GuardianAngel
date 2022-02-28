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

#include "DateTime.hpp"
#include "DateTimeFactory.hpp"

TEST(Time24Ben, BensCase) {
	std::shared_ptr<Time24> t = Time24Factory::create(1,2,3);

	std::string tToString = t->toString();

	std::cerr << "toString: " << tToString << std::endl;

	ASSERT_EQ(" 1:02:03", tToString);
}

TEST(DateTime, ToString) {
	std::shared_ptr<DateTime> obj = DateTimeFactory::create("21/11/29,22:23:57+04");

	std::string tToString = obj->toString();

	std::cerr << "toString: " << tToString << std::endl;
}
