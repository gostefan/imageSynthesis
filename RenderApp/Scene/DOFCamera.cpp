
#include "DOFCamera.h"

#include <SamplingApp/Sampler/RandomSampler.h>
#include <SamplingApp/Warping/UniformDiskWarping.h>

DOFCamera::DOFCamera(unsigned resX, unsigned resY, float sharpDistance, float discRadius) :
		Camera(resX, resY), sharpDistance(sharpDistance), discRadius(discRadius) {
	randomSampler = new RandomSampler();
	discWarping = new UniformDiskWarping();
}

DOFCamera::~DOFCamera() {
	delete randomSampler;
	delete discWarping;
}

void DOFCamera::generateRay(Ray * r, float x, float y) const
{
	// Assume x in 0:res_x - 1 and y in 0:res_y-1
	
	//the preallocated Ray r should have a world space origin and direction
	//and as well as a region of interest (using r->tMin and r->tMax)
	//given the image space coordinates x and y.

	r->o = this->m_cameraToWorld.colAsVec3(3);
	Math::Vec3f point = m_WindowToWorld * Math::Vec3f(x+0.5f, y+0.5f, 0);

	r->d = (point - r->o).normalized();
	point = r->o + sharpDistance*r->d;

	Vec2f sample;
	randomSampler->generateSamples(1, &sample);
	Vec3f warped = discWarping->warp(sample) * discRadius;
	r->o = m_WindowToWorld * (warped);

	r->d = (point - r->o).normalized();
}