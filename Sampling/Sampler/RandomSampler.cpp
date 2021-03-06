
#include "RandomSampler.h"

#include <Math/Rand.h>

namespace {
	const char* NAME = "Random";
}

/**
 * Generates uniformly distributed Samples in the range [0,1].
 *
 */
void RandomSampler::generateSamples(size_t nSamplesSqrt, std::vector<Math::Vec2f>& sampleStorage) {
	for (size_t i = 0; i < nSamplesSqrt*nSamplesSqrt; i++) {
		const float x = static_cast<float>(rand()) / RAND_MAX;
		const float y = static_cast<float>(rand()) / RAND_MAX;
		sampleStorage[i] = Math::Vec2f(x, y);
	}
}

const char* RandomSampler::getName() {
	return NAME;
}