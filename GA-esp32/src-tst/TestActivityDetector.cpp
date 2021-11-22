/*
 * TestActivityDetector.cpp
 *
 *  Created on: 22 nov. 2021
 *      Author: wilbert
 */

#include <ActivityDetectorFactory.hpp>
#include <ActivityDetectorImpl.hpp>
#include <gtest/gtest.h>
#include <stddef.h>
#include <memory>
#include <iostream>

int *sampleSrc;

int sample() {
	auto r = *sampleSrc;
	sampleSrc++;
//	std::cerr << "Sample: " << r << std::endl;
	return r;
}

TEST(ActivityDetector, NoActivity) {

	int activity[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	sampleSrc = activity;
	auto ad = ActivityDetectorFactory::create(sample);
	auto adTick = (ActivityDetectorImpl*) ad.get();

	size_t nrSamples = 0;
	while (nrSamples < sizeof(activity) / sizeof(int) - 1) {
		adTick->doTick();
		ASSERT_EQ(0, ad->getNrActivations());

		nrSamples++;
	}
}

TEST(ActivityDetector, NoActivityWithNoise) {

	int activity[] = { 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0 };

	sampleSrc = activity;
	auto ad = ActivityDetectorFactory::create(sample);
	auto adTick = (ActivityDetectorImpl*) ad.get();

	size_t nrSamples = 0;
	while (nrSamples < sizeof(activity) / sizeof(int) - 1) {
		adTick->doTick();
		ASSERT_EQ(0, ad->getNrActivations());

		nrSamples++;
	}
}

TEST(ActivityDetector, ActivityWithNoNoise) {
	// Note that the first entry of activity is already consumed during creation of the Debouncer.
	// This also means that the while loop should run one iteration less.
	int activity[] = { 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1 };

	sampleSrc = activity;
	auto ad = ActivityDetectorFactory::create(sample);
	auto adTick = (ActivityDetectorImpl*) ad.get();

	size_t nrSamples = 0;
	while (nrSamples < sizeof(activity) / sizeof(int) - 1) {
		adTick->doTick();

		if (nrSamples < 7) {
			ASSERT_EQ(0, ad->getNrActivations());
		}
		if (nrSamples >= 7) {
			ASSERT_EQ(1, ad->getNrActivations());
		}
		nrSamples++;
	}
	ad->clearActivity();
	ASSERT_EQ(0, ad->getNrActivations());

}

TEST(ActivityDetector, ActivityWithNoiseAndBackToIdle) {

	int activity[] = { 0,
			0, 1, 1, 1, 1, 1, // Period of activity
			1, 1, 1, 0, 0, 0, 0, 0, // Period of activity followed by idle
			0, 1, 1, 1,	1, 1, // Period of idle followed by activity
			1, 1, 1 };

	sampleSrc = activity;
	auto ad = ActivityDetectorFactory::create(sample);
	auto adTick = (ActivityDetectorImpl*) ad.get();

	size_t nrSamples = 0;
	while (nrSamples < sizeof(activity) / sizeof(int) - 1) {
		adTick->doTick();

//		std::cerr << ". " << nrSamples << ", " << ad->getNrActivations()
//				<< std::endl;

		if (nrSamples < 6) {
			ASSERT_EQ(0, ad->getNrActivations());
		}
		if (nrSamples >= 6 && nrSamples <20) {
			ASSERT_EQ(1, ad->getNrActivations());
		}
		if (nrSamples >=20) {
			ASSERT_EQ(2, ad->getNrActivations());
		}

		nrSamples++;
	}
}

