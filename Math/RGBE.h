/*! \file RGBE.h
    \author Wojciech Jarosz
*/
#ifndef IMG_RGBE_H
#define IMG_RGBE_H

#include <Math/Color.h>

namespace Math
{
class RGBE;

void Color3ToRGBE(unsigned char rgbe[4], const Color3f& c);
void RGBEToColor3(Color3f* result, const unsigned char rgbe[4]);

//! Color packed as 4 bytes
class RGBE
{
public:
    unsigned char r, g, b, e;
    
    RGBE() : r(0), g(0), b(0), e(0) {}
    
    RGBE(float red, float green, float blue)
    {
        Color3ToRGBE(&r, Color3f(red, green, blue));
    }
    
    RGBE(const Color3f& c) {Color3ToRGBE(&r, c);}

    RGBE(const RGBE& rgbe) :
        r(rgbe.r), g(rgbe.g), b(rgbe.b), e(rgbe.e)
    {
        // empty
    }
    
    operator Color3f() const
    {
        Color3f ret;
        RGBEToColor3(&ret, &r);
        return ret;
    }
    
    void fromColor3(const Color3f& c)
    {
        Color3ToRGBE(&r, c);
    }
    
    void toColor3(Color3f* result) const
    {
        RGBEToColor3(result, &r);
    }

    static const float s_ldexp[256];
};


inline void
RGBEToFloat(float *r, float *g, float *b, const unsigned char rgbe[4])
{
    float f = RGBE::s_ldexp[rgbe[3]];
    *r = (rgbe[0] + 0.5f) * f;
    *g = (rgbe[1] + 0.5f) * f;
    *b = (rgbe[2] + 0.5f) * f;
}


inline void 
floatToRGBE(unsigned char rgbe[4], float red, float green, float blue)
{
    float v;
    int e;
    
    v = red;
    if(green > v)
        v = green;
    if(blue > v)
        v = blue;

    if(v < 1e-32f)
        rgbe[0] = rgbe[1] = rgbe[2] = rgbe[3] = 0;
    else
    {
        v = frexp(v, &e) * 256.0f/v;
        rgbe[0] = (unsigned char)(red * v);
        rgbe[1] = (unsigned char)(green * v);
        rgbe[2] = (unsigned char)(blue * v);
        rgbe[3] = (unsigned char)(e + 128);
    }
}


inline void
Color3ToRGBE(unsigned char rgbe[4], const Color3f& c)
{
    floatToRGBE(rgbe, c[0], c[1], c[2]);
}

inline void
RGBEToColor3(Color3f* result, const unsigned char rgbe[4])
{
    RGBEToFloat(&(*result)[0], &(*result)[1], &(*result)[2], rgbe);
}


} // namespace Math

#endif // IMG_RGBE_H
