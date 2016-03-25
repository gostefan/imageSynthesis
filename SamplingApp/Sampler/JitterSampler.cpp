
#include "JitterSampler.h"

/**
 * Generates uniformly distributed with jitter Samples in the range [0,1].
 *
 */
void JitterSampler::generateSamples(int nSamplesSqrt, Math::Vec2f* sampleStorage) {
	float spacing = 1.f/(nSamplesSqrt);
	for (int i = 0; i < nSamplesSqrt; i++) {
		for (int j = 0; j < nSamplesSqrt; j++) {
			float x = static_cast<float>(rand()) / RAND_MAX;
			float y = static_cast<float>(rand()) / RAND_MAX;
			sampleStorage[i+nSamplesSqrt*j] = Math::Vec2f(spacing * (i + x), spacing * (j + y));
		}
	}
}