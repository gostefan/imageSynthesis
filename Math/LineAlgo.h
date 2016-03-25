/*! \file LineAlgo.h
    \brief Contains extra Line algorithms.
    \author Wojciech Jarosz
*/
#ifndef MATH_LINE_ALGO_H
#define MATH_LINE_ALGO_H

#include <Math/Line.h>

namespace Math
{

//Not anymore Templated Nd Box-Ray Intersection of a ray (ro,rd) and a box, within tMin and tMax
//Returns distances to the intersections hitt0 and hitt1
inline bool
intersects(const Vec3f & ro, const Vec3f & rd, float time, const Box4f& box,
           float tMin = Limits<float>::min(),
           float tMax = Limits<float>::max(),
           float * hitt0 = 0,
           float * hitt1 = 0)
{
	// Exclude wrong time
	if (time < box.min[3] || time > box.max[3]) {
		return false;
	}
	// Exclude wrong vector direction
    for (unsigned i = 0; i < 3; ++i)
    {
        // Update interval for ith bounding box slab
        float invRayDir = 1 / rd[i];
        float tNear = (box.min[i] - ro[i]) * invRayDir;
        float tFar  = (box.max[i] - ro[i]) * invRayDir;
    
        // Update parametric interval from slab intersection ts
        if (tNear > tFar)
            std::swap(tNear, tFar);
        tMin = tNear > tMin ? tNear : tMin;
        tMax = tFar  < tMax ? tFar  : tMax;
        if (tMin > tMax)
            return false;
    }

    if (hitt0)
        *hitt0 = tMin;
    if (hitt1)
        *hitt1 = tMax;
    return true;
}

//Templated Intersection test between a ray (ro,rd) and a triangle (A,B,C)
//in the interval t0,t1. Returns distance t, uv coordinates and the geometric normal Ng
template <typename T>
inline bool
intersect(const Vec3<T> & ro, const Vec3<T> & rd,
          const Vec3<T>& A,
          const Vec3<T>& B,
          const Vec3<T>& C,
          T t0, T t1,
          T * t, Vec2<T> * uv, Vec3<T> * Ng)
{
    Vec3<T> AB = B - A;
    Vec3<T> AC = C - A;
    Vec3<T> N = cross(AB, AC);
    T det = dot(rd, N);
    
    if (det == T(0))
       return false;
        
    T detInv = T(1) / det;
    Vec3<T> toA = ro - A;

    T a = dot(rd, cross(toA, AC)) * detInv;
    if (a < T(0))
        return false;

    T b = dot(rd, cross(AB, toA)) * detInv;
    if (b < T(0) || a + b > T(1))
        return false;

    T tempT = -dot(toA, N) * detInv;
    if (tempT < t0 || tempT > t1)
        return false;

    *t = tempT;
    uv->set(a, b);
    *Ng = N;
    return true;
}


//Templated (fast) Intersection test between a ray (ro,rd) and a triangle (A,B,C)
//in the interval t0,t1. Returns only true or false.
template <typename T>
inline bool
intersects(const Vec3<T> & ro, const Vec3<T> & rd,
           const Vec3<T>& A,
           const Vec3<T>& B,
           const Vec3<T>& C,
           T t0, T t1)
{
    Vec3<T> AB = B - A;
    Vec3<T> AC = C - A;
    Vec3<T> N = cross(AB, AC);
    float det = dot(rd, N);

    if (det == T(0))
       return false;

    T detInv = 1.0f / det;
    Vec3<T> toA = ro - A;

    T a = dot(rd, cross(toA, AC)) * detInv;
    if (a < T(0))
        return false;

    T b = dot(rd, cross(AB, toA)) * detInv;
    if (b < T(0) || a + b > T(1))
        return false;

    T t = -dot(toA, N) * detInv;
    if (t < t0 || t > t1)
        return false;

    return true;
}

} // namespace Math

#endif  // MATH_LINE_ALGO_H
