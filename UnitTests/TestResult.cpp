#include "TestResult.h"

namespace test {
	TestResult::TestResult() :
		TestResult(0, 0, 0) {
	}
	TestResult::TestResult(uint32_t nFailure, uint32_t nError, uint32_t nSuccess) {
		this->nFailure = nFailure;
		this->nError = nError;
		this->nSuccess = nSuccess;
	}

	TestResult& TestResult::operator+(const TestResult& other) {
		this->nFailure += other.nFailure;
		this->nError += other.nError;
		this->nSuccess += other.nSuccess;
		return *this;
	}

	std::ostream & operator<<(std::ostream & out, const TestResult& res) {
		out << "Success: " << res.nSuccess << " | Error: " << res.nError << " | Failure: " << res.nFailure;
		return out;
	}
}