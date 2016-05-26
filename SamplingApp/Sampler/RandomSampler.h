
#pragma once

#include "Sampler.h"

#include <vector>

class RandomSampler : public Sampler {
	virtual void generateSamples(int nSamplesSqrt, std::vector<Math::Vec2f>& sampleStorage) override;
};