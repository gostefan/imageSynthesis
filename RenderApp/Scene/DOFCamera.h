/*! \file Camera.h
 \brief Contains the Camera class
 \author Wojciech Jarosz
*/
#pragma once

#include "Camera.h"

#include <memory>

class Sampler;
class Warping;

class DOFCamera : public Camera {
	protected:
		float sharpDistance;
		float discRadius;

		std::unique_ptr<Sampler> randomSampler;
		std::unique_ptr<Warping> discWarping;

	public:
		DOFCamera(unsigned resX = 640, unsigned resY = 480, float sharpDistance = 1, float discRadius = 1);
		~DOFCamera();

		virtual void generateRay(Ray * r, float x, float y) const;
};