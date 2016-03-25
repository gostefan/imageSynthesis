
#include "Sampler.h"

class UniformSampler : public Sampler {
	virtual void generateSamples(int nSamplesSqrt, Math::Vec2f* sampleStorage);
};