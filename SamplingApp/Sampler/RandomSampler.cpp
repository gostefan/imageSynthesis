
#include "RandomSampler.h"

/**
 * Generates uniformly distributed Samples in the range [0,1].
 *
 */
void RandomSampler::generateSamples(int nSamplesSqrt, Math::Vec2f* sampleStorage) {
	for (int i = 0; i < nSamplesSqrt*nSamplesSqrt; i++) {
		float x = static_cast<float>(rand()) / RAND_MAX;
		float y = static_cast<float>(rand()) / RAND_MAX;
		sampleStorage[i] = Math::Vec2f(x, y);
	}
}