/*! \file Box.h
    \brief Contains the definition of Box class.
    \author Wojciech Jarosz
*/
#ifndef MATH_BOX_H
#define MATH_BOX_H

#include <Math/Fwd.h>
#include <Math/LimitsT.h>
#include <Math/Core.h>
#include <Math/Vec4.h>
#include <iostream>

namespace Math
{

//! Represents a bounded interval in higher dimensions.
/*!
    Box is an N-D interval. It assumes that the template parameter Vec
    provides a dimensions() member(such as Math::Vec2 or Math::Vec3)
*/
template <typename Vec>
class Box
{
public:
    Vec min;            //!< The lower-bound of the interval
    Vec max;            //!< The upper-bound of the interval


    //-----------------------------------------------------------------------
    //@{ \name Constructors
    //-----------------------------------------------------------------------
    Box() {makeEmpty();}
    Box(const Vec & point) : min(point), max(point) {}
    Box(const Vec & minT, const Vec & maxT) : min(minT), max(maxT) {}
    template <typename T>
    Box(const Box<T> & box) : min((Vec)box.min), max((Vec)box.max) {}
    template <typename T, typename S>
    Box(const Box<T> & box1, const Box<S> & box2) :
        min((Vec)box1.min), max((Vec)box1.max)
    {
        enclose((Box<Vec>)box2);
    }
    //@}

    
    //-----------------------------------------------------------------------
    //@{ \name Equality and inequality
    //-----------------------------------------------------------------------
    bool operator==(const Box<Vec> & src) const
        {return(min == src.min && max == src.max);}
    bool operator!=(const Box<Vec> & src) const
        {return(min != src.min || max != src.max);}
    //@}

    
    //-----------------------------------------------------------------------
    //@{ \name Box manipulation
    //-----------------------------------------------------------------------
    void makeEmpty()
    {
        min = Vec(Limits<typename Vec::BaseType>::max());
        max = Vec(Limits<typename Vec::BaseType>::min());
    }
    void enclose(const Vec & point)
    {
        for (unsigned i = 0; i < Vec::dimensions(); i++)
        {
            min[i] = std::min(point[i], min[i]);
            max[i] = std::max(point[i], max[i]);
        }
    }
    void enclose(const Box<Vec> & box)
    {
        for (unsigned i = 0; i < Vec::dimensions(); i++)
        {
            min[i] = std::min(box.min[i], min[i]);
            max[i] = std::max(box.max[i], max[i]);
        }
    }
    void intersect(const Box<Vec> & box)
    {
        for (unsigned i = 0; i < Vec::dimensions(); i++)
        {
            min[i] = std::max(box.min[i], min[i]);
            max[i] = std::min(box.max[i], max[i]);
        }
    }
    void moveMinTo(const Vec &newMin)
    {
        Vec diff(newMin - min);
        min = newMin;
        max += diff;
    }
    void moveMaxTo(const Vec &newMax)
    {
        Vec diff(newMax - max);
        max = newMax;
        min += diff;
    }
    void makeValid()
    {
        for (unsigned i = 0; i < Vec::dimensions(); i++)
        {
            if (min[i] > max[i])
                std::swap(min[i], max[i]);
        }
    }
    //@}

    
    //-----------------------------------------------------------------------
    //@{ \name Query functions
    //-----------------------------------------------------------------------
    Vec  size() const {return max-min;}
    Vec  center() const {return(max+min)/2;}
    bool contains(const Vec & point, bool proper = false) const
    {
        if (proper)
        {
            for (unsigned i = 0; i < Vec::dimensions(); i++)
                if (point[i] <= min[i] || point[i] >= max[i])
                    return false;
            return true;
        }
        else
        {
            for (unsigned i = 0; i < Vec::dimensions(); i++)
                if (point[i] < min[i] || point[i] > max[i])
                    return false;
            return true;
        }
    }
    bool intersects(const Box<Vec> & box, bool proper = false) const
    {
        if (proper)
        {
            for (unsigned i = 0; i < Vec::dimensions(); i++)
                if (box.max[i] <= min[i] || box.min[i] >= max[i])
                    return false;
            return true;
        }
        else
        {
            for (unsigned i = 0; i < Vec::dimensions(); i++)
                if (box.max[i] < min[i] || box.min[i] > max[i])
                    return false;
            return true;
        }
    }
    typename Vec::BaseType volume() const
    {
        typename Vec::BaseType retVal(1);
        Vec s = size();
        for (unsigned i = 0; i < Vec::dimensions(); i++)
            retVal *= s[i];
        return retVal;
    }
    typename Vec::BaseType area() const
    {
        typename Vec::BaseType retVal(0);
        Vec s = size();
        for (unsigned i = 0; i < Vec::dimensions(); i++)
            for (unsigned j = i+1; j < Vec::dimensions(); j++)
                retVal += s[i]*s[j];
        return 2*retVal;
    }
    unsigned majorAxis() const
    {
        unsigned major = 0;
        Vec s = size();
    
        for (unsigned i = 1; i < Vec::dimensions(); i++)
            if (s[i] > s[major])
                major = i;
        return major;
    }
    void boundingSphere(Vec* center, float* radius) const
    {
        *center = 0.5f *(min + max);
        *radius = intersects(*center) ?(*center - max).length() : 0.0f;
    }
    //@}


    //-----------------------------------------------------------------------
    //@{ \name Classification
    //-----------------------------------------------------------------------
    bool hasVolume() const
    {
        for (unsigned i = 0; i < Vec::dimensions(); i++)
            if (max[i] <= min[i])
                return false;
        return true;
    }
    bool isEmpty() const
    {
        for (unsigned i = 0; i < Vec::dimensions(); i++)
            if (max[i] < min[i])
                return true;
        return false;
    }
    //@}
};


template <typename Vec>
inline std::ostream&
operator<<(std::ostream &o, const Box<Vec> &b)
{
    return o << "[(" << b.min << "),(" << b.max << ")]";
}


} // namespace Math

#endif // MATH_BOX_H
