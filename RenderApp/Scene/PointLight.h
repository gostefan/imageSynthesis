
#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "Light.h"
#include "../Geometry/BBox.h"
#include "LightRay.h"

class Sampler;
class Warping;

class PointLight : public Light
{
protected:
	Math::Vec3f m_position;
	Math::Color3f m_power;
	Sampler* randomSampler;
	Warping* sphereWarping;

public:
	PointLight(const Math::Vec3f & position = Math::Vec3f(), Math::Color3f & power = Math::Color3f(1,1,1));
	virtual ~PointLight();

	virtual bool isRenderable() const;

    virtual bool intersect(Ray * r) const;
	
    virtual void fillHitInfo(Ray * r) const;
	
	virtual Math::Color3f getPower();
	
	virtual Math::Vec3f getPosition();

	virtual Math::Vec3f evalP(float u, float v) const;
	virtual Math::Vec3f evalN(float u, float v) const;
	virtual void renderGL() const;
	virtual BBox getBBox() const;
	virtual BBox getBBox(float uStart, float uEnd, float vStart, float vEnd) const;
	virtual void getIrradianceSamples(Vec3f point, const Scene* scene, vector<LightRay>& result, float time);
	virtual TracePhoton samplePhoton();
};

#endif