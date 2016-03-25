/*! \file Warp.h
    \brief Contains mappings between various useful domains.
    \author Wojciech Jarosz
*/
#ifndef MATH_WARP_H
#define MATH_WARP_H

#include <Math/Vec2.h>
#include <Math/Vec3.h>
#include <Math/Core.h>

namespace Math
{

//! Encapsulates various useful mappings
/*!
    This class provides functions to map from various useful domains, such as
    disks, spheres, hemispheres, etc.
    
    You may find the following useful for reference:
    
        Shirley, Pete. "Nonuniform Random Points Via Warping."
            Graphics Gems III. pp. 80--83
    and
        Dutré, Philip. "Global Illumination Compendium." 
            http://www.cs.kuleuven.ac.be/~phil/GI/
            
    among other sources.
        
*/
class Warp
{
public:
    //-----------------------------------------------------------------------
    //@{ \name Mappings to the 2D square.
    //-----------------------------------------------------------------------
    static void  uniformSquare(Vec2f* v, float s, float t);
    static float uniformSquarePdf();
    //@}


    //-----------------------------------------------------------------------
    //@{ \name Disk.
    //-----------------------------------------------------------------------
    static void  uniformDisk(Vec2f* v, float s, float t);
    static float uniformDiskPdf();
    //@}
    
    static void  uniformCylinder(Vec3f* v, float s, float t);
    static float uniformCylinderPdf();


    //-----------------------------------------------------------------------
    //@{ \name Mappings to sections of the 3D sphere.
    //-----------------------------------------------------------------------
    static void  uniformSphere(Vec3f* v, float, float);
    static float uniformSpherePdf();

    static void  uniformSphericalCap(Vec3f* v, float, float, float);
    static float uniformSphericalCapPdf(float cosThetaMax);
    
    static void  uniformHemisphere(Vec3f* v, float s, float t);
    static float uniformHemispherePdf();
    
    static void  cosineHemisphere(Vec3f* v, float s, float t);
    static float cosineHemispherePdf(const Vec3f& v);
    
    static void  phongHemisphere(Vec3f* v, float s, float t, float n);
    static float phongHemispherePdf(const Vec3f& v, float n);
    //@}
};


inline void
Warp::uniformSquare(Vec2f* v, float s, float t)
{
    v->x = s;
    v->y = t;
}


inline float
Warp::uniformSquarePdf()
{
    return 1.0f;
}


inline void
Warp::uniformDisk(Vec2f* v, float s, float t)
{
	// TODO: implement
}


inline float
Warp::uniformDiskPdf()
{
	// TODO: implement
    return 1.0f;
}


inline void
Warp::uniformCylinder(Vec3f* v, float s, float t)
{
    // TODO: implement
}


inline float
Warp::uniformCylinderPdf()
{
	// TODO: implement
	return 1.0f;
}


//! Samples a unit sphere uniformily.
/*!
    Generates a direction on the unit sphere proportional to solid angle.
    
    Uses Archimedes Theorem to sample the cylinder and then projects back
    onto the sphere.
*/
inline void
Warp::uniformSphere(Vec3f* v, float s, float t)
{
    // TODO: implement
}


inline float
Warp::uniformSpherePdf()
{
	// TODO: implement
	return 1.0f;
}


inline void
Warp::uniformSphericalCap(Vec3f* v, float s, float t, float cosThetaMax)
{
	// TODO: implement
}


inline float
Warp::uniformSphericalCapPdf(float cosThetaMax)
{
	// TODO: implement
	return 1.0f;
}


//! Samples a unit hemisphere uniformily.
/*!
    Generates a direction on the unit hemisphere uniformily distributed wrto
    solid angle.
*/
inline void
Warp::uniformHemisphere(Vec3f* v, float s, float t)
{
	// TODO: implement
}


inline float
Warp::uniformHemispherePdf()
{
	// TODO: implement
	return 1.0f;
}


//! Samples a cosine-weighted hemisphere.
/*!
    Generates a direction on the unit hemisphere distributed proportional
    to cosine-weighted solid angle.
    
    The technique used is to just use spherical coordinates directly.
*/
inline void
Warp::cosineHemisphere(Vec3f* v, float s, float t)
{
	// TODO: implement
}

inline float
Warp::cosineHemispherePdf(const Vec3f& v)
{
	// TODO: implement
	return 1.0f;
}


//! Samples a phong-weighted hemisphere.
/*!
    Generates a direction on the unit hemisphere distributed proportional
    to cosine^n-weighted solid angle.
*/
inline void
Warp::phongHemisphere(Vec3f* v, float s, float t, float n)
{
	// TODO: implement
}


inline float
Warp::phongHemispherePdf(const Vec3f& v, float n)
{
	// TODO: implement
	return 1.0f;
}


} // namespace Math

#endif // MATH_WARP_H
