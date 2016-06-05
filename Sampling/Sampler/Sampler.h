
#pragma once

#include<Math/Vec2.h>

#include<vector>

class Sampler {
	public:
		virtual void generateSamples(size_t nSamplesSqrt, std::vector<Math::Vec2f>& sampleStorage) = 0;
		virtual const char* getName() = 0;
};