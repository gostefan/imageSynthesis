//
//  Light.h
//  Image Synthesis Basecode
//
//  Created by Stefan Götschi on 02/27/12
//  Copyright 2012 Student. All rights reserved.
//

#pragma once

#include <Math/Color.h>
#include <Img/Fwd.h>
#include "HitInfo.h"
#include "../Geometry/Shape.h"
#include "LightRay.h"

#include <stack>

class Ray;

struct TracePhoton {
	TracePhoton(Vec3f origin, Vec3f direction, Color3f power) : origin(origin), direction(direction), power(power) {};
	Vec3f origin, direction;
	Color3f power;
	bool specular;
	stack<float> refractionIndex;
};

class Light : virtual public Shape {
	public:
		virtual ~Light() { };
	
		/*!
			Intersect the specified primitive in local space.

			\param r ray in the object's local space
			\param primID primitive index to intersect
		*/
		virtual bool intersect(Ray* r) const = 0;
	
		virtual void fillHitInfo(Ray* r) const = 0;

		virtual Math::Color3f getPower() = 0;

		virtual Math::Vec3f getPosition() = 0;

		virtual void getIrradianceSamples(Vec3f point, const Scene* scene, vector<LightRay>& result, float time) = 0;

		virtual TracePhoton samplePhoton() = 0;
};