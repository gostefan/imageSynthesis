
#include "UniformSampler.h"

/**
 * Generates uniformly distributed Samples in the range [0,1].
 *
 */
void UniformSampler::generateSamples(int nSamplesSqrt, Math::Vec2f* sampleStorage) {
	float spacing = 1.f/(2*nSamplesSqrt);
	for (int i = 0; i < nSamplesSqrt; i++) {
		for (int j = 0; j < nSamplesSqrt; j++) {
			sampleStorage[i+nSamplesSqrt*j] = Math::Vec2f(spacing * (2*i + 1), spacing * (2*j + 1));
		}
	}
}