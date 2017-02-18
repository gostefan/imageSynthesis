#pragma once

#include "TestResult.h"

namespace test {
	class TreeTests {
	public:
		static TestResult allTests();
		static TestResult trivialSplits();
	};
}