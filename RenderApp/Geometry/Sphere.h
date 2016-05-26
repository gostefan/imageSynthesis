
#pragma once

#include <Math/Vec3.h>

#include "Shape.h"
#include "BBox.h"

using namespace std;

class Displacement;

class Sphere : virtual public Shape {
	protected:
		Math::Vec3f location;
		float radius;

	public:
		Sphere(SurfaceShader* surfaceShader = 0, const Math::Vec3f & loc = Math::Vec3f(0.0f), float rad = 1.0f, Motion* motion = 0, Displacement* disp = 0);

		float getRadius() {
			return radius;
		}

		Math::Vec3f getPosition() {
			return location;
		}
	
		void renderGL() const;
	
		bool intersect(Ray* r) const;
		virtual void fillHitInfo(Ray* r) const;
	
		Math::Vec3f evalP(float u, float v) const;
		Math::Vec3f evalN(float u, float v) const;

		virtual BBox getBBox() const;
		virtual BBox getBBox(float uStart, float uEnd, float vStart, float vEnd) const;
};
