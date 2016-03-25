/*! \file Camera.h
 \brief Contains the Camera class
 \author Wojciech Jarosz
*/
#ifndef DOF_CAMERA_H
#define DOF_CAMERA_H

#include "Camera.h"

class Sampler;
class Warping;

class DOFCamera : public Camera
{
protected:
	float sharpDistance;
	float discRadius;

	Sampler* randomSampler;
	Warping* discWarping;

public:
	DOFCamera(unsigned resX = 640, unsigned resY = 480, float sharpDistance = 1, float discRadius = 1);
	~DOFCamera();

    virtual void generateRay(Ray * r, float x, float y) const;
};

#endif