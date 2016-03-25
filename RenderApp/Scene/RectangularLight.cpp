
#include <Math/MathGL.h>
#include "RectangularLight.h"
#include "../Shader/LightShader.h"
#include "SamplingApp/Sampler/RandomSampler.h"
#include "SamplingApp/Warping/CosineHemisphereWarping.h"

RectangularLight::RectangularLight(Math::Color3f power_in, SurfaceShader* surfaceShader, const Math::Vec3f& minLoc, const Math::Vec2f& size_in, unsigned int nSamplesSqrt_in, Displacement* displacement) : 
minPosition(minLoc), size(size_in), nSamplesSqrt(nSamplesSqrt_in), nSamples(nSamplesSqrt_in*nSamplesSqrt_in), power(power_in) {
	randomSampler = new RandomSampler();
	cosineWarping = new CosineHemisphereWarping();
	radiance = power/(size.x * size.y);
	Shape::surfaceShader = new LightShader(radiance, surfaceShader);
}

RectangularLight::~RectangularLight() {
	delete surfaceShader;
	delete randomSampler;
	delete cosineWarping;
}

void RectangularLight::getIrradianceSamples(Vec3f point, const Scene* scene, vector<LightRay>& result, float time) {
	// Draw new samples
	Vec2f* drawnPoints = new Vec2f[nSamples];
	randomSampler->generateSamples(nSamplesSqrt, drawnPoints);
	
	Color3f sum = Color3f(0);
	for (unsigned int i = 0; i < nSamples; i++) {
		// Warp and rotate samples
		Vec3f warpedPoint = Vec3f(size.x*drawnPoints[i].x, 0, size.y*drawnPoints[i].y);
		
		Vec3f difference = point - (warpedPoint + minPosition);
		float distance = difference.length();

		LightRay lr;
		Ray r;
		lr.direction = difference / distance;
		r.d = lr.direction;
		r.o = warpedPoint + minPosition;
		r.tMax = distance - r.tMin;
		r.time = time;

		// Check for intersection
		for (unsigned int k = 0; k < scene->shapes.size(); k++) {
			scene->shapes[k]->intersect(&r);
		}

		//if ray hit something then it does not contribute
		if (r.hit.shape == 0) {
			float pdf = 1/(size.x*size.y);
			lr.radiance = power * max(0.f,dot(lr.direction, Vec3f(0,-1,0))) / (2*static_cast<float>(M_PI)*pow(distance, 2));
		}
		else {
			lr.radiance = Color3f(0);
		}
		result.push_back(lr);
	}
	delete drawnPoints;	
}

Math::Color3f RectangularLight::getPower() {
	return power;
}

Math::Vec3f RectangularLight::getPosition() {
	return minPosition;
}

bool RectangularLight::intersect(Ray* r) const {
	float yDist = minPosition.y - r->o.y;
	float dist = yDist / r->d.y;
	if (r->tMin < dist && r->tMax > dist) {
		Vec3f pos = r->o + dist * r->d;
		if (pos.x > minPosition.x && pos.x < (minPosition.x + size.x)
				&& pos.z > minPosition.z && pos.z < (minPosition.z + size.y)) {
			r->tMax = dist;
			fillHitInfo(r);
			return true;
		}
		return false;
	}
	return false;
}

void RectangularLight::fillHitInfo(Ray * r) const {
	double hitDistance = r->tMax;
	r->hit.shape = this;
	r->hit.surfaceShader = this->surfaceShader;

	// This is calculated only here because the multiplication is more efficient than the division
	r->hit.t = static_cast<float>(hitDistance);
	Math::Vec3f hitPoint = r->o + hitDistance * r->d;

	r->hit.P = hitPoint;
	r->hit.N = Vec3f(0,-1,0);
	r->hit.Ng = r->hit.N;

	r->hit.I = r->d;
}

Math::Vec3f RectangularLight::evalP(float u, float v) const {
	return Vec3f(u*size.x + minPosition.x, v*size.y + minPosition.y, minPosition.z);
}

Math::Vec3f RectangularLight::evalN(float u, float v) const {
	return Vec3f(0,0,1);
}

BBox RectangularLight::getBBox() const {
	return BBox(minPosition, minPosition + Vec3f(size.x, size.y, 0));
}

BBox RectangularLight::getBBox(float uMin, float uMax, float vMin, float vMax) const {
	return BBox(minPosition + Vec3f(uMin*size.x, vMin*size.y, 0), minPosition + Vec3f(uMax*size.x, vMax*size.y, 0));
}

void RectangularLight::renderGL() const {
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex(minPosition);
	glVertex(Vec3f(minPosition.x + size.x, minPosition.y, minPosition.z));
	glVertex(Vec3f(minPosition.x + size.x, minPosition.y, minPosition.z));
	glVertex(Vec3f(minPosition.x + size.x, minPosition.y, minPosition.z + size.y));
	glVertex(Vec3f(minPosition.x + size.x, minPosition.y, minPosition.z + size.y));
	glVertex(Vec3f(minPosition.x, minPosition.y, minPosition.z + size.y));
	glVertex(Vec3f(minPosition.x, minPosition.y, minPosition.z + size.y));
	glVertex(minPosition);
	glEnd();
}

TracePhoton RectangularLight::samplePhoton() {
	Vec2f samples[2];
	randomSampler->generateSamples(1,&samples[0]);
	randomSampler->generateSamples(1,&samples[1]);

	Mat44f rot = Mat44f::I();
	rot.rotateTo(Vec3f(0,0,1), Vec3f(0,-1,0));
	
	Vec3f warpedPoint = Vec3f(size.x*samples[0].x + minPosition.x, minPosition.y, size.y*samples[0].y + minPosition.z);
	Vec3f warpedDirection = rot * cosineWarping->warp(samples[1]);

	TracePhoton photon = TracePhoton(warpedPoint, warpedDirection, power);

	return photon;
}