
#pragma once

#include<Math/Vec2.h>

#include<vector>

class Sampler {
	public:
		virtual void generateSamples(int nSamplesSqrt, std::vector<Math::Vec2f>& sampleStorage) = 0;
};