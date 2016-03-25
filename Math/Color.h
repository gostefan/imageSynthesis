/*! \file Color.h
    \brief Contains the definition of Color classes
    \author Wojciech Jarosz
*/
#ifndef IMG_COLOR_H
#define IMG_COLOR_H

#include <Math/Fwd.h>
#include <iostream>
#include <math.h>
#include <Math/Vec3.h>
#include <Math/Vec4.h>

namespace Math
{

void XYZToRGB(float *R, float *G, float *B, float X, float Y, float z);
void RGBToXYZ(float *X, float *Y, float *Z, float R, float G, float B);
void XYZToLab(float *L, float *a, float *b, float X, float Y, float Z);
void LabToXYZ(float *X, float *Y, float *Z, float L, float a, float b);
void XYZToLuv(float *L, float *u, float *v, float X, float Y, float Z);
void LuvToXYZ(float *X, float *Y, float *Z, float L, float u, float v);
void XYZToxyY(float *x, float *y, float *YO, float X, float Y, float Z);
void xyYToXYZ(float *X, float *YO, float *Z, float x, float y, float Y);
void RGBToHLS(float *H, float *L, float *S, float R, float G, float B);
void HLSToRGB(float *R, float *G, float *B, float H, float L, float S);
void RGBToHSV(float *H, float *S, float *V, float R, float G, float B);
void HSVToRGB(float *R, float *G, float *B, float H, float S, float V);
	
//! A tristimulus color class.
/*!
	This class handles storing and manipulating colors composed of 3 components.
 */
template <typename T>
class Color3 : public Vec3<T>
{
public:

    //-----------------------------------------------------------------------
    //@{ \name Constructors and assignment
    //-----------------------------------------------------------------------
    Color3() : Vec3<T>() {}
    Color3(const Color3 & c) : Vec3<T>(c) {}
    Color3(T a, T b, T c) : Vec3<T>(a, b, c) {}
    explicit Color3(T a) : Vec3<T>(a) {}
	template <typename S>
    Color3(const S* v) : Vec3<T>(v) {}
	template <typename S>
	Color3(const Vec3<S> &v): Vec3 <T> (v) {}
    //@}


    //-----------------------------------------------------------------------
    //@{ \name Casting operators.
    //-----------------------------------------------------------------------
    operator const T*() const {return Vec3<T>::toArray();}
    operator T*() {return Vec3<T>::toArray();}
    //@}


    //-----------------------------------------------------------------------
    //@{ \name Addition.
    //-----------------------------------------------------------------------
    Color3<T> operator+(const Color3& c) const
    {
        return Color3(*(const Vec3<T> *)this + (const Vec3<T> &)c);
    }
    const Color3<T> & operator+=(const Color3<T>& c)
    {
        *((Vec3<T> *) this) += c; return *this;
    }
    const Color3<T> & operator+=(T a)
    {
        *((Vec3<T> *) this) += a; return *this;
    }
    //@}


    //-----------------------------------------------------------------------
    //@{ \name Subtraction.
    //-----------------------------------------------------------------------
    Color3<T> operator-(const Color3<T>& c) const
    {
        return Color3(*(const Vec3<T> *)this - (const Vec3<T> &)c);
    }
    const Color3<T> & operator-=(float a)
    {
        *((Vec3<T> *) this) -= a; return *this;
    }
    const Color3<T> & operator-=(const Color3<T>& c)
    {
        *((Vec3<T> *) this) -= c; return *this;
    }
    //@}


    //-----------------------------------------------------------------------
    //@{ \name Multiplication.
    //-----------------------------------------------------------------------
    Color3<T> operator*(T a) const
    {
        return Color3(*(const Vec3<T> *)this * a);
    }
    Color3<T> operator*(const Color3& c) const
    {
        return Color3(*(const Vec3<T> *)this * (const Vec3<T> &)c);
    }
    const Color3<T> & operator*=(T a)
    {
        *((Vec3<T> *) this) *= a; return *this;
    }
    const Color3<T> & operator*=(const Color3& c)
    {
        *((Vec3<T> *) this) *= c; return *this;
    }
    Color3<T> operator-() const {return Color3(-(*(const Vec3<T> *)this));}
    const Color3<T> & negate() {((Vec3<T> *) this)->negate(); return *this;}
    //@}


    //-----------------------------------------------------------------------
    //@{ \name Division.
    //-----------------------------------------------------------------------
    Color3<T> operator/(T a) const
    {
        return Color3(*(const Vec3<T> *)this / a);
    }
    Color3<T> operator/(const Color3<T> & c) const
    {
        return Color3(*(const Vec3<T> *)this / (const Vec3<T> &)c);
    }
    const Color3<T> & operator/=(T a)
    {
        *((Vec3<T> *) this) /= a; return *this;
    }
    const Color3<T> & operator/=(const Color3<T> & c)
    {
        *((Vec3<T> *) this) /= c; return *this;
    }
    //@}
    
    T average() const {return (Vec3<T>::x + Vec3<T>::y + Vec3<T>::z) / T(3);}
    T luminance() const
    {
        return T(0.212671) * Vec3<T>::x +
			   T(0.715160) * Vec3<T>::y +
			   T(0.072169) * Vec3<T>::z;
    }
    Color3 pow(const Color3& exp) const
    {
        Color3 res;
        for (int i = 0; i < 3; ++i)
            res[i] = (*this)[i] > T(0) ? Math::MathT<T>::pow((*this)[i], exp[i]) : T(0);
        return res;
    }

    Color3 RGBToXYZ() const
    {
        Color3 ret;
        Math::RGBToXYZ(&ret[0], &ret[1], &ret[2], Vec3<T>::x, Vec3<T>::y, Vec3<T>::z);
        return ret;
    }
    Color3 XYZToRGB() const
    {
        Color3 ret;
        Math::XYZToRGB(&ret[0], &ret[1], &ret[2], Vec3<T>::x, Vec3<T>::y, Vec3<T>::z);
        return ret;
    }
    Color3 XYZToLab() const
    {
        Color3 ret;
        Math::XYZToLab(&ret[0], &ret[1], &ret[2], Vec3<T>::x, Vec3<T>::y, Vec3<T>::z);
        return ret;
    }
    Color3 LabToXYZ() const
    {
        Color3 ret;
        Math::LabToXYZ(&ret[0], &ret[1], &ret[2], Vec3<T>::x, Vec3<T>::y, Vec3<T>::z);
        return ret;
    }
    Color3 XYZToLuv() const
    {
        Color3 ret;
        Math::XYZToLuv(&ret[0], &ret[1], &ret[2], Vec3<T>::x, Vec3<T>::y, Vec3<T>::z);
        return ret;
    }
    Color3 LuvToXYZ() const
    {
        Color3 ret;
        Math::LuvToXYZ(&ret[0], &ret[1], &ret[2], Vec3<T>::x, Vec3<T>::y, Vec3<T>::z);
        return ret;
    }
    Color3 xyYToXYZ() const
    {
        Color3 ret;
        Math::xyYToXYZ(&ret[0], &ret[1], &ret[2], Vec3<T>::x, Vec3<T>::y, Vec3<T>::z);
        return ret;
    }
    Color3 XYZToxyY() const
    {
        Color3 ret;
        Math::XYZToxyY(&ret[0], &ret[1], &ret[2], Vec3<T>::x, Vec3<T>::y, Vec3<T>::z);
        return ret;
    }
    
    Color3 RGBToHSV() const
    {
        Color3 ret;
        Math::RGBToHSV(&ret[0], &ret[1], &ret[2], Vec3<T>::x, Vec3<T>::y, Vec3<T>::z);
        return ret;
    }
    Color3 HSVToRGB() const
    {
        Color3 ret;
        Math::HSVToRGB(&ret[0], &ret[1], &ret[2], Vec3<T>::x, Vec3<T>::y, Vec3<T>::z);
        return ret;
    }
    
    Color3 RGBToHLS() const
    {
        Color3 ret;
        Math::RGBToHLS(&ret[0], &ret[1], &ret[2], Vec3<T>::x, Vec3<T>::y, Vec3<T>::z);
        return ret;
    }
    Color3 HLSToRGB() const
    {
        Color3 ret;
        Math::HLSToRGB(&ret[0], &ret[1], &ret[2], Vec3<T>::x, Vec3<T>::y, Vec3<T>::z);
        return ret;
    }
};

template <typename T>
inline Color3<T>
exp(const Color3<T> & c)
{
    return Color3<T>(Math::MathT<T>::exp(c[0]),
					 Math::MathT<T>::exp(c[1]),
					 Math::MathT<T>::exp(c[2]));
}
	
template <typename T>
inline Color3<T>
fastExp(const Color3<T> & c)
{
    return Color3<T>(Math::fastExp(c[0]),
					 Math::fastExp(c[1]),
					 Math::fastExp(c[2]));
}

	
template <typename T>
inline Color3<T>
log(const Color3<T> & c)
{
    return Color3<T>(Math::MathT<T>::log(c[0]),
					 Math::MathT<T>::log(c[1]),
					 Math::MathT<T>::log(c[2]));
}

	
template <typename T, typename S>
inline Color3<T>
powf(const Color3<T> & c, T e)
{
    return Color3<T>(Math::MathT<T>::pow(c[0], e),
					 Math::MathT<T>::pow(c[1], e),
					 Math::MathT<T>::pow(c[2], e));
}
	
	
	
template <typename T, typename S>
inline Color3<T>
operator * (S x, const Color3<T> &v)
{
	return Color3<T>(x * v.x, x * v.y, x * v.z);
}

	
//! A tristimulus plus alpha color class.
/*!
	This class handles storing and manipulating colors composed of 3 components
	with an additional alpha component.
 */
template <typename T>
class Color4 : public Vec4<T>
{
public:
    
    
    //-----------------------------------------------------------------------
    //@{ \name Constructors and assignment
    //-----------------------------------------------------------------------
    Color4() : Vec4<T>()  {}
    Color4(const Color3<T> & c, T a) : Vec4<T>(c, a) {}
    Color4(const Color4 & c) : Vec4<T>(c) {}
    Color4(T a, T b, T c, T d) : Vec4<T>(a, b, c, d) {}
    explicit Color4(T a) : Vec4<T>(a) {}
	template <typename S>
    Color4(const S* v) : Vec4<T>(v) {}
	template <typename S>
	Color4(const Vec4<S> &v): Vec4<T> (v) {}
    //@}
	
	
    //-----------------------------------------------------------------------
    //@{ \name Casting operators.
    //-----------------------------------------------------------------------
    operator const T*() const {return (const T*)&Vec4<T>::x;}
    operator T*() {return (T*)&Vec4<T>::x;}
    //@}
	
	//-----------------------------------------------------------------------
	//@{ \name Addition.
	//-----------------------------------------------------------------------
    Color4<T> operator+(const Color4& c) const
    {
        return Color4(*(const Vec4<T> *)this + (const Vec4<T> &)c);
    }
    const Color4<T> & operator+=(const Color4<T>& c)
    {
        *((Vec4<T> *) this) += c; return *this;
    }
    const Color4<T> & operator+=(T a)
    {
        *((Vec4<T> *) this) += a; return *this;
    }
    //@}
	
	
    //-----------------------------------------------------------------------
    //@{ \name Subtraction.
    //-----------------------------------------------------------------------
    Color4<T> operator-(const Color4<T>& c) const
    {
        return Color4(*(const Vec4<T> *)this - (const Vec4<T> &)c);
    }
    const Color4<T> & operator-=(float a)
    {
        *((Vec4<T> *) this) -= a; return *this;
    }
    const Color4<T> & operator-=(const Color4<T>& c)
    {
        *((Vec4<T> *) this) -= c; return *this;
    }
    //@}
	
	
    //-----------------------------------------------------------------------
    //@{ \name Multiplication.
    //-----------------------------------------------------------------------
    Color4<T> operator*(T a) const
    {
        return Color4(*(const Vec4<T> *)this * a);
    }
    Color4<T> operator*(const Color4& c) const
    {
        return Color4(*(const Vec4<T> *)this * (const Vec4<T> &)c);
    }
    const Color4<T> & operator*=(T a)
    {
        *((Vec4<T> *) this) *= a; return *this;
    }
    const Color4<T> & operator*=(const Color4& c)
    {
        *((Vec4<T> *) this) *= c; return *this;
    }
    Color4<T> operator-() const {return Color4(-(*(const Vec4<T> *)this));}
    const Color4<T> & negate() {((Vec4<T> *) this)->negate(); return *this;}
    //@}
	
	
    //-----------------------------------------------------------------------
    //@{ \name Division.
    //-----------------------------------------------------------------------
    Color4<T> operator/(T a) const
    {
        return Color4(*(const Vec4<T> *)this / a);
    }
    Color4<T> operator/(const Color4<T> & c) const
    {
        return Color4(*(const Vec4<T> *)this / (const Vec4<T> &)c);
    }
    const Color4<T> & operator/=(T a)
    {
        *((Vec4<T> *) this) /= a; return *this;
    }
    const Color4<T> & operator/=(const Color4<T> & c)
    {
        *((Vec4<T> *) this) /= c; return *this;
    }
    //@}
    

    T average() const
    {
        return T(Vec4<T>::x + Vec4<T>::y + Vec4<T>::z + Vec4<T>::w) / T(4);
    }
};


inline void
XYZToRGB(float *R, float *G, float *B, float X, float Y, float Z)
{
    // [ R ]   [  3.240479 -1.537150 -0.498535 ]   [ X ]
    // [ G ] = [ -0.969256  1.875992  0.041556 ] * [ Y ]
    // [ B ]   [  0.055648 -0.204043  1.057311 ]   [ Z ]
     
    *R =  3.240479f * X - 1.537150f * Y - 0.498535f * Z;
    *G = -0.969256f * X + 1.875992f * Y + 0.041556f * Z;
    *B =  0.055648f * X - 0.204043f * Y + 1.057311f * Z;
}

inline void
RGBToXYZ(float *X, float *Y, float *Z, float R, float G, float B)
{
    // [ X ]   [  0.412453  0.357580  0.180423 ]   [ R ]
    // [ Y ] = [  0.212671  0.715160  0.072169 ] * [ G ]
    // [ Z ]   [  0.019334  0.119193  0.950227 ]   [ B ]
     
    *X = 0.412453f * R + 0.357580f * G + 0.180423f * B;
    *Y = 0.212671f * R + 0.715160f * G + 0.072169f * B;
    *Z = 0.019334f * R + 0.119193f * G + 0.950227f * B;
}

} // namespace Img

#endif // RB_IMAGE_COLOR_H
