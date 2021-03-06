
#include "UniformSampler.h"

namespace {
	const char* NAME = "Uniform";
}

/**
 * Generates uniformly distributed Samples in the range [0,1].
 *
 */
void UniformSampler::generateSamples(size_t nSamplesSqrt, std::vector<Math::Vec2f>& sampleStorage) {
	float spacing = 1.f/(2*nSamplesSqrt);
	for (size_t i = 0; i < nSamplesSqrt; i++) {
		for (size_t j = 0; j < nSamplesSqrt; j++) {
			sampleStorage[i+nSamplesSqrt*j] = Math::Vec2f(spacing * (2*i + 1), spacing * (2*j + 1));
		}
	}
}

const char* UniformSampler::getName() {
	return NAME;
}