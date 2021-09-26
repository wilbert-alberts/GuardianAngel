/*
 * DebouncerTest.cpp
 *
 *  Created on: 26 sep. 2021
 *      Author: wilbert
 */

#include "gtest/gtest.h"
#include "Debouncer.hpp"

#include <iostream>

void TestDebouncer(int nrInputs, int inputValues[], bool expTickOutputs[], int expOutputs[])
{
	int idx = 0;
	auto debouncer = new Debouncer(5, [&]() -> int {
		return inputValues[idx++];
	});
	bool tickValue[nrInputs] = { false };
	int outputValues[nrInputs] = { -1 };

	for (int i = 0; i < nrInputs; i++) {
		tickValue[i] = debouncer->tick();
		outputValues[i] = debouncer->getValue();
	}

	for (int i = 0; i < nrInputs; i++) {
		std::cout << i << ": input: " << inputValues[i] << ", tickOutput: " << tickValue[i] << ", output: " << outputValues[i] << std::endl;
		ASSERT_EQ(expTickOutputs[i], tickValue[i]);
		ASSERT_EQ(expOutputs[i], outputValues[i]);
	}
}

TEST(Debouncer, StableTransition) {
	int inputValues[] = { 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	int nrInputs = sizeof(inputValues) / sizeof(int);

	bool expectedTickValues[nrInputs] = { false };
	int expectedOutputValues[nrInputs] = { 0 };

	expectedTickValues[11] = true;
	for (int i = 11; i < nrInputs; i++)
		expectedOutputValues[i] = 1;

	TestDebouncer(nrInputs, inputValues, expectedTickValues, expectedOutputValues);

}
