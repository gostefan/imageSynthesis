
#pragma once

#include "Sampler.h"

#include <vector>

class UniformSampler : public Sampler {
	virtual void generateSamples(int nSamplesSqrt, std::vector<Math::Vec2f>& sampleStorage) override;
	virtual const char* getName() override;
};