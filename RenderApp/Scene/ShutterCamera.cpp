
#include "ShutterCamera.h"

ShutterCamera::ShutterCamera(float shutterTime, std::unique_ptr<Camera>&& recursiveCamera) :
		Camera(recursiveCamera->xRes(), recursiveCamera->yRes()), shutterTime(shutterTime), recursiveCamera(std::move(recursiveCamera)) { }

void ShutterCamera::generateRay(Ray * r, float x, float y) const {
	recursiveCamera->generateRay(r, x, y);
	r->time = (static_cast<float>(rand())/RAND_MAX) * shutterTime;
}