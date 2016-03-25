//
//  Light.h
//  Image Synthesis Basecode
//
//  Created by Stefan Götschi on 02/27/12
//  Copyright 2012 Student. All rights reserved.
//

#ifndef RECT_LIGHT_H
#define RECT_LIGHT_H

#include "Light.h"

class Ray;
class Scene;
class Sampler;
class Warping;

class RectangularLight : virtual public Light
{
public:
	RectangularLight(Math::Color3f power, SurfaceShader* surfaceShader = 0, const Math::Vec3f& minLoc = Math::Vec3f(0), const Math::Vec2f& size = Math::Vec2f(1), unsigned int nSamplesSqrt = 4, Displacement* disp = 0);

	virtual ~RectangularLight();

	virtual Math::Color3f getPower();

	virtual Math::Vec3f getPosition();

	virtual void getIrradianceSamples(Vec3f point, const Scene* scene, vector<LightRay>& result, float time);
	
	virtual bool intersect(Ray * r) const;
	virtual void fillHitInfo(Ray * r) const;
	virtual void renderGL() const;
	virtual Math::Vec3f evalP(float u, float v) const;
	virtual Math::Vec3f evalN(float u, float v) const;
	virtual BBox getBBox() const;
	virtual BBox getBBox(float uStart, float uEnd, float vStart, float vEnd) const;
	virtual TracePhoton samplePhoton();

protected:
	Math::Color3f radiance;
	Math::Color3f power;
	Math::Vec3f minPosition;
	Math::Vec2f size;
	unsigned int nSamplesSqrt;
	unsigned int nSamples;
	Sampler* randomSampler;
	Warping* cosineWarping;
};

#endif
