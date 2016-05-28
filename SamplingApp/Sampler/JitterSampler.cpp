
#include "JitterSampler.h"

#include <Math/Rand.h>

namespace {
	const char* NAME = "Jitter";
}

/**
 * Generates uniformly distributed with jitter Samples in the range [0,1].
 *
 */
void JitterSampler::generateSamples(size_t nSamplesSqrt, std::vector<Math::Vec2f>& sampleStorage) {
	float spacing = 1.f/(nSamplesSqrt);
	for (size_t i = 0; i < nSamplesSqrt; i++) {
		for (size_t j = 0; j < nSamplesSqrt; j++) {
			const float x = static_cast<float>(rand()) / RAND_MAX;
			const float y = static_cast<float>(rand()) / RAND_MAX;
			sampleStorage[i+nSamplesSqrt*j] = Math::Vec2f(spacing * (i + x), spacing * (j + y));
		}
	}
}

const char* JitterSampler::getName() {
	return NAME;
}