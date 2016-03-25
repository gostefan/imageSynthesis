/*! \file Ray.cpp
    \brief Implementation of the Ray class.
    \author Wojciech Jarosz
*/
#if HAVE_CONFIG_H
#  include <config.h>
#endif // HAVE_CONFIG_H

#include "Ray.h"
#include <Math/Core.h>
#include <Math/LimitsT.h>

#if defined(_WIN32)
#define isnan _isnan
#endif


/*!
    Creates a ray.
*/
Ray::Ray() :
    Math::Line3f(), tMin(0.001f), tMax(Math::Limits<float>::max()), time(0)
{
	refractionIndex.push(1);
    // empty
}


/*!
    Creates a child ray.
    \param origin The initial origin of Ray, defaults to an uninitialized
                    Vec3f.
    \param direction The initial direction of Ray, defaults to an
                    uninitialized Vec3f.
    \param r The parent ray.
    
    The remaining parameters for the ray are automatically inferred from the
    parent ray.
*/
Ray::Ray(const Math::Vec3f & origin,
         const Math::Vec3f & direction, const float time) :
    Math::Line3f(origin, direction),
    tMin(0.001f), tMax(Math::Limits<float>::max()), time(time)
{
	refractionIndex.push(1);
    // empty
}

