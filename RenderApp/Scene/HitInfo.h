/*! \file HitInfo.h
    \brief Contains the definition of the HitInfo class.
    \author Wojciech Jarosz
*/
#ifndef CORE_HIT_INFO_H
#define CORE_HIT_INFO_H

#include <Math/Color.h>
#include <Math/Vec2.h>
#include <Math/Vec3.h>

class Shape;
class SurfaceShader;

//! Contains information about a ray hit with a surface.
/*!
    HitInfos are used by object intersection routines. They are useful in
    order to return more than just the hit distance.
*/
class HitInfo
{
public:
    const Shape * shape;				//!< The geometric primitive at the hit point.
    const SurfaceShader * surfaceShader;//!< The shader at the hit point.
	
	float time;
    float t;							//!< The hit distance
    Math::Vec3f I;						//!< The "incident" vector,
										//!< point from viewer position to the
										//!< shading position P
    Math::Vec3f P;						//!< The hit point.
    Math::Vec2f st;						//!< Texture coordinates
    Math::Vec2f uv;						//!< Texture coordinates
    Math::Vec3f N;						//!< Shading normal vector.
    Math::Vec3f Ng;						//!< Geometric normal vector.

    Math::Vec3f dPdu, dPdv;				//!< Derivatives of P wrt uv
    Math::Vec3f dNdu, dNdv;				//!< Derivatives of N wrt uv

    //! Default constructor.
    explicit HitInfo(const SurfaceShader * sShader = 0,
                     float t = 0.0f,
                     const Math::Vec3f& P = Math::Vec3f(0.0f),
                     const Math::Vec2f& uv = Math::Vec2f(0.0f),
                     const Math::Vec3f& N = Math::Vec3f(0, 1, 0),
                     const Math::Vec3f& Ng = Math::Vec3f(0, 1, 0)) :
        shape(0), surfaceShader(sShader),
        t(t), P(P), uv(uv), N(N), Ng(Ng),
        dPdu(0.0f), dPdv(0.0f), dNdu(0.0f), dNdv(0.0f)
    {
        // empty
    }
};

#endif // CORE_HIT_INFO_H
