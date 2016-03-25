//
//  Light.h
//  Image Synthesis Basecode
//
//  Created by Stefan Götschi on 02/27/12
//  Copyright 2012 Student. All rights reserved.
//

#ifndef SPHERE_LIGHT_H
#define SPHERE_LIGHT_H

#include "Light.h"
#include "../Geometry/Sphere.h"

class Ray;
class Scene;

class SphereLight : virtual public Light, virtual public Sphere
{
public:
	SphereLight(Math::Color3f power, SurfaceShader* surfaceShader = 0, const Math::Vec3f& loc = Math::Vec3f(0.0f), float radius = 1.0f, unsigned int nSamplesSqrt = 4, Displacement* disp = 0);

	virtual ~SphereLight();

	virtual Math::Color3f getPower();

	virtual Math::Vec3f getPosition();

	virtual void getIrradianceSamples(Vec3f point, const Scene* scene, vector<LightRay>& result, float time) = 0;
	
	virtual bool intersect(Ray * r) const {
		return Sphere::intersect(r);
	}
	virtual void fillHitInfo(Ray * r) const {
		Sphere::fillHitInfo(r);
	}

	virtual TracePhoton samplePhoton() {
		//assert(false);
		return TracePhoton(location, Vec3f(0,0,1), Color3f(0,0,0));
	}

	virtual void renderGL() const {
		Sphere::renderGL();
	}
	virtual Math::Vec3f evalP(float u, float v) const {
		return Sphere::evalP(u,v);
	}
	virtual Math::Vec3f evalN(float u, float v) const {
		return Sphere::evalN(u,v);
	}
	virtual BBox getBBox() const {
		return Sphere::getBBox();
	}
	virtual BBox getBBox(float uStart, float uEnd, float vStart, float vEnd) const {
		return Sphere::getBBox(uStart, uEnd, vStart, vEnd);
	}

protected:
	Math::Color3f radiance;
	Math::Color3f power;
	unsigned int nSamplesSqrt;
	unsigned int nSamples;
};

#endif
