#include "Sphere.h"
#include <OGL/Primitive.h>
#include <Math/MathGL.h>
#include <Math/Core.h>
#include "Motion.h"

using namespace Math;

Sphere::Sphere(SurfaceShader * ss, const Vec3f & loc, float rad, Motion* motion, Displacement* displ) :
	Shape(ss, motion, displ),
	location(loc),
	radius(rad)
{
}

void
Sphere::renderGL() const
{
	glPushMatrix();
		glTranslate(location);
		glutWireSphere(radius, 16, 16);
	glPopMatrix();
}

bool
Sphere::intersect(Ray * r) const
{
	Vec3f translLocation = location;
	if (motion != 0) {
		translLocation += motion->getTranslation(r->time);
	}
	
	Vec3f dist = translLocation - r->o;
	float distOnRay = dot(dist, r->d);
	Vec3f closest = dist - r->d * distOnRay;
	if (closest.length2() <= radius * radius && (distOnRay >= 0 || dist.length2() < radius * radius)) {
		// distOnRay >= 0 needed for leaving rays.
		double cosAngle = closest.length() / radius;
		double sinAngle = sqrt(1 - pow(cosAngle, 2)); // should be the same... sin(acos(cosAngle));
		double t = distOnRay - sinAngle * radius;
		if (t < r->tMin)
			// The other intersection
			t = distOnRay + sinAngle * radius;
		if (t > r->tMin && t < r->tMax /*&& t > 0*/) {
			// Should be ensured by t > ray.min_t
			r->tMax = static_cast<float>(t);
			this->fillHitInfo(r);
			return true;
		}
	}
	return false;
}


void
Sphere::fillHitInfo(Ray * r) const
{
	Vec3f translLocation = location;
	if (motion != 0) {
		translLocation += motion->getTranslation(r->time);
	}

	double hitDistance = r->tMax;
	r->hit.shape = this;
	r->hit.surfaceShader = this->surfaceShader;

	// This is calculated only here because the multiplication is more efficient than the division
	r->hit.t = static_cast<float>(hitDistance);
	Math::Vec3f hitPoint = r->o + hitDistance * r->d;

	r->hit.P = hitPoint;
	r->hit.N = (hitPoint - translLocation).normalized();
	r->hit.Ng = r->hit.N;

	r->hit.I = r->d;
	r->hit.time = r->time;

	if (r->hit.N.y > 0.5)
		int a = 0;
	Vec3f v = Vec3f(0,0,1);
	v = (v - dot(v, r->hit.N)*r->hit.N).normalized();
	Vec3f u = cross(v, r->hit.N);
	r->hit.dPdu = u;
	r->hit.dPdv = v;
}

Vec3f Sphere::evalP(float u, float v) const
{
	// u is phi and v is theta

	float phi = u * 2*PI;
	float theta = (v - .5f) * PI;

	return Vec3f(location.x + radius * cos(phi) * cos(theta), location.y + radius * sin(phi) * cos(theta), location.z + radius * sin(theta));
}

Vec3f Sphere::evalN(float u, float v) const
{
	float phi = u * 2*PI;
	float theta = (v - .5f) * PI;

	return Vec3f(cos(phi) * cos(theta), sin(phi) * cos(theta), sin(theta));
}

BBox Sphere::getBBox() const {
	Vec3f additor = Vec3f(radius);
	if (displacement != 0) {
		Vec3f additor = Vec3f(radius + displacement->getScale());
	}
	return BBox(location - additor, location + additor);
};

BBox Sphere::getBBox(float uStart, float uEnd, float vStart, float vEnd) const {
	// check for displacement
	float displScale = 0;
	if (displacement != 0) {
		displScale = displacement->getScale();
	}
		

	float maxCosTheta = 1;
	// only if both values of v lie in one hemisphere we use their cosines
	if (vStart >= 0.5 && vEnd <= 0.5) {
		maxCosTheta = max(cos((vStart - .5f) * PI), cos((vEnd - .5f) * PI));
	}

	// Calculate start and end points
	Vec3f start = evalP(uStart, vStart);
	Vec3f end = evalP(uEnd, vEnd);
	Vec3f nStart = evalN(uStart, vStart) * displScale;
	Vec3f nEnd = evalN(uEnd, vEnd) * displScale;

	// Calculate max/min including the possibility of displacement
	Vec3f upper(max(start.x + max(-nStart.x, nStart.x), end.x + max(-nEnd.x, nEnd.x)), max(start.y + max(-nStart.y, nStart.y), end.y + max(-nEnd.y, nEnd.y)), max(start.z + max(-nStart.z, nStart.z), end.z + max(-nEnd.z, nEnd.z)));
	Vec3f lower(min(start.x + min(-nStart.x, nStart.x), end.x + min(-nEnd.x, nEnd.x)), min(start.y + min(-nStart.y, nStart.y), end.y + min(-nEnd.y, nEnd.y)), min(start.z + min(-nStart.z, nStart.z), end.z + min(-nEnd.z, nEnd.z)));

	// Include missed extremal points
	// 0/2Pi is never missed
	if (uStart < 0.25 && uEnd > 0.25) {
		upper.y = max(upper.y, location.y + maxCosTheta * (radius + displScale));
	}
	if (uStart < 0.5 && uEnd > 0.5) {
		lower.x = min(lower.x, location.x - maxCosTheta * (radius + displScale));
	}
	if (uStart < 0.75 && uEnd > 0.75) {
		lower.y = max(lower.y, location.y - maxCosTheta * (radius + displScale));
	}

	return BBox(lower, upper);
};