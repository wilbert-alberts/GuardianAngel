/*
 * TestConfig.cpp
 *
 *  Created on: 7 nov. 2021
 *      Author: wilbert
 */

#include "gtest/gtest.h"
#include "Config.hpp"
#include "ConfigFactory.hpp"

#include <iostream>

const std::string v1Value = "v1 value with (=) equals";
const std::string v2Value = "v2 value";

TEST(Config, SetGetProperty) {

	auto cfg = ConfigFactory::create();

	cfg->putProperty("prop1", v1Value);
	cfg->putProperty("prop2", v2Value);

	ASSERT_EQ(*(cfg->getProperty("prop1")), v1Value);
	ASSERT_EQ(*(cfg->getProperty("prop2")), v2Value);
	ASSERT_EQ((cfg->getProperty("prop3")), nullptr);

	cfg->putProperty("prop1", "v11");
	ASSERT_EQ(*(cfg->getProperty("prop1")), "v11");
	ASSERT_EQ(*(cfg->getProperty("prop2")), v2Value);
	ASSERT_EQ((cfg->getProperty("prop3")), nullptr);
}

TEST(Config, SaveLoadProperty) {
	auto cfg = ConfigFactory::create();

	cfg->putProperty("prop1", v1Value);
	cfg->putProperty("prop2", v2Value);

	cfg->saveProperties();

	cfg->clear();
	ASSERT_EQ((cfg->getProperty("prop1")), nullptr);
	ASSERT_EQ((cfg->getProperty("prop2")), nullptr);
	ASSERT_EQ((cfg->getProperty("prop3")), nullptr);

	cfg->loadProperties();

	ASSERT_EQ(*(cfg->getProperty("prop1")), v1Value);
	ASSERT_EQ(*(cfg->getProperty("prop2")), v2Value);
	ASSERT_EQ((cfg->getProperty("prop3")), nullptr);

}

