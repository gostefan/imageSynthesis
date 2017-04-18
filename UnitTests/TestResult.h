#pragma once

#include "..\Math\stdint.h"

#include <iostream>

namespace test {
	class TestResult {
		public:
			TestResult();
			TestResult(uint32_t nFailure, uint32_t nError, uint32_t nSuccess);

			TestResult& operator+(const TestResult& other);
		protected:
			uint32_t nFailure;
			uint32_t nError;
			uint32_t nSuccess;

			friend std::ostream & operator<<(std::ostream & out, const TestResult& res);
	};

	std::ostream& operator<<(std::ostream& out, const TestResult& res);
}