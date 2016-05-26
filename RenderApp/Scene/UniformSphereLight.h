//
//  Light.h
//  Image Synthesis Basecode
//
//  Created by Stefan Götschi on 02/27/12
//  Copyright 2012 Student. All rights reserved.
//

#pragma once

#include "SphereLight.h"
#include "../Scene/LightRay.h"

#include <memory>
#include <vector>

class Ray;
class Sampler;
class Scene;
class Warping;

class UniformSphereLight : public SphereLight {
	public:
		UniformSphereLight(Math::Color3f power, SurfaceShader* surfaceShader, const Math::Vec3f& location, float radius, unsigned int nSamplesSqrt, Displacement* displacement);
		virtual ~UniformSphereLight();

		virtual Math::Color3f getPower();

		virtual void getIrradianceSamples(Vec3f point, const Scene* scene, vector<LightRay>& result, float time);

	protected:
		std::unique_ptr<Sampler> randomSampler;
		std::unique_ptr<Warping> sphereWarping;
};
