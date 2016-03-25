/*! \file Ray.h
    \brief Definition of the Ray class.
    \author Wojciech Jarosz
*/
#ifndef CORE_RAY_H
#define CORE_RAY_H

#include "HitInfo.h"
#include <Math/Vec3.h>
#include <Math/Line.h>
#include <Math/LimitsT.h>
#include <stack>

//! Definition of a general ray class.
/*!
    A Ray is used to intersect geometric primatives. Rays can be used to
    represent eye rays, as well as light rays (photons). A Ray consists of a
    ray origin, and a ray direction. In addition to this, a ray contains some
    extra information that is useful for ray tracing certain types of
    primatives. A ray also keeps track of the number of reflective, refractive,
    and diffuse surfaces that it has hit.
*/
class Ray : public Math::Line3f
{
public:

    explicit Ray();
    Ray(const Math::Vec3f& o, const Math::Vec3f& d, const float time = 0);
    
    float tMin, tMax;                  //!< Region of interest
	float time;
	std::stack<float> refractionIndex;

    HitInfo hit;                       //!< Information about the hitpoint
};

#endif // CORE_RAY_H
