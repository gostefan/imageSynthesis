
#include <Math/Rand.h>

#include "Sampler.h"

class JitterSampler : public Sampler {
	virtual void generateSamples(int nSamplesSqrt, Math::Vec2f* sampleStorage);
};