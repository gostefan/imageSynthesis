/*!\file Color.cpp
   \author Wojciech Jarosz
 */
#if HAVE_CONFIG_H
#  include <config.h>
#endif // HAVE_CONFIG_H

#include <Math/Color.h>
#include <Math/Core.h>
#include <Math/MathT.h>
#include <Math/FastMath.h>

namespace Img
{

using namespace Math;

namespace
{
const float eps = 216.0f / 24389.0f;
const float kappa = 24389.0f / 27.0f;
const float refX = 0.950456f;
const float refZ = 1.08875f;
const float refU = (4.0f * refX) / (refX + 15.0f + 3.0f * refZ);
const float refV = 9.0f / (refX + 15.0f + 3.0f * refZ);
} // namespace

void
XYZToLab(float *L, float *a, float *b, float X, float Y, float Z)
{
    X *= 1.0f / 0.950456f;
    Z *= 1.0f / 1.08875f;

    X = (X > eps) ? cbrtf(X) : (kappa * X + 16.0f) / 116.0f;
    Y = (Y > eps) ? cbrtf(Y) : (kappa * Y + 16.0f) / 116.0f;
    Z = (Z > eps) ? cbrtf(Z) : (kappa * Z + 16.0f) / 116.0f;
    
    *L = (116.0f * Y) - 16;
    *a = 500.0f * (X - Y);
    *b = 200.0f * (Y - Z);
}


void
LabToXYZ(float *X, float *Y, float *Z, float L, float a, float b)
{
    float yr = (L > kappa*eps) ? pow3((L + 16.0f) / 116.0f) : L / kappa;
    float fy = (yr > eps) ? (L + 16.0f) / 116.0f : (kappa*yr + 16.0f) / 116.0f;
    float fx = a / 500.0f + fy;
    float fz = fy - b / 200.0f;
    
    float fx3 = Math::pow3(fx);
    float fz3 = Math::pow3(fz);
    
    *X = (fx3 > eps) ? fx3 : (116.0f * fx - 16.0f) / kappa;
    *Y = yr;
    *Z = (fz3 > eps) ? fz3 : (116.0f * fz - 16.0f) / kappa;
    
    *X *= 0.950456f;
    *Z *= 1.08875f;
}


void
XYZToLuv(float *L, float *u, float *v, float X, float Y, float Z)
{
    float denom = 1.0f / (X + 15.0f * Y + 3.0f * Z);
    *u = (4.0f * X) * denom;
    *v = (9.0f * Y) * denom;

    *L = (Y > eps) ? (116.0f * cbrtf(Y)) - 16.0f : kappa * Y;
    *u = 13.0f * *L * (*u - refU);
    *v = 13.0f * *L * (*v - refV);
}


void
LuvToXYZ(float *X, float *Y, float *Z, float L, float u, float v)
{
    *Y = (L > kappa * eps) ? Math::pow3((L + 16.0f) / 116.0f) : L / kappa;
    
    float a = (1.0f/3.0f) * ((52.0f * L) / (u + 13.0f * L * refU) - 1.0f);
    float b = -5.0f * *Y;
    float d = *Y * ((39.0f * L) / (v + 13.0f * L * refV) - 5.0f);
    
    *X = (d - b) / (a + (1.0f/3.0f));
    *Z = *X * a + b;
}



void
XYZToxyY(float *x, float *y, float *YO, float X, float Y, float Z)
{
    float denom = X + Y + Z;
    if (denom == 0.0f)
    {
        // set chromaticity to D65 whitepoint
        *x = 0.31271f;
        *y = 0.32902f;
    }
    else
    {
        *x = X / denom;
        *y = Y / denom;
    }
    *YO = Y;
}


void
xyYToXYZ(float *X, float *YO, float *Z, float x, float y, float Y)
{
    if (Y == 0.0f)
    {
        *X = 0.0f;
        *Z = 0.0f;
    }
    else
    {
        *X = x*Y;
        *Z = (1.0f - x - y) * Y / y;
    }
    *YO = Y;
}



//! Convert a color in RGB colorspace to an equivalent color in HSV colorspace.
/*!
    This is derived from sample code in:

    Foley et al. Computer Graphics: Principles and Practice.
        Second edition in C. 592-596. July 1997.
*/
void
RGBToHSV(float *H, float *S, float *V, float R, float G, float B)
{
    // Calculate the max and min of red, green and blue.
    float max = (R > G) ? ((R > B) ? R : B) : ((G > B) ? G : B);
    float min = (R < G) ? ((R < B) ? R : B) : ((G < B) ? G : B);
    float delta = max - min;

    // Set the saturation and value
    *S = (max != 0) ? (delta)/max : 0;
    *V = max;

    if (*S == 0.0f)
        *H = 0.0f;
    else
    {
        if (R == max)
            *H = (G - B)/delta;

        else if (G == max)
            *H = 2 + (B - R)/delta;

        else if (B == max)
            *H = 4 + (R - G)/delta;

        *H /= 6.0f;

        if (*H < 0.0f)
            *H += 1.0f;
    }
}


//! Convert a color in HSV colorspace to an equivalent color in RGB colorspace.
/*!
    This is derived from sample code in:

    Foley et al. Computer Graphics: Principles and Practice.
        Second edition in C. 592-596. July 1997.
*/
void
HSVToRGB(float *R, float *G, float *B, float H, float S, float V)
{
    if (S == 0.0f)
    {
        // achromatic case
        *R = *G = *B = V;
    }
    else
    {
        if (H == 1.0f)
            H = 0.0f;
        else
            H *= 6.0f;
    
        int i = floor2Int(H);
        float f = H-i;
        float p = V* (1-S);
        float q = V* (1-(S*f));
        float t = V* (1-(S* (1-f)));
    
        switch (i) 
        {
            case 0: *R = V; *G = t; *B = p; break;
            case 1: *R = q; *G = V; *B = p; break;
            case 2: *R = p; *G = V; *B = t; break;
            case 3: *R = p; *G = q; *B = V; break;
            case 4: *R = t; *G = p; *B = V; break;
            case 5: *R = V; *G = p; *B = q; break;
        }
    }
}


//! Convert a color in RGB colorspace to an equivalent color in HLS colorspace.
/*!
    This is derived from sample code in:

    Foley et al. Computer Graphics: Principles and Practice.
        Second edition in C. 592-596. July 1997.
*/
void
RGBToHLS(float *H, float *L, float *S, float R, float G, float B)
{
    // Calculate the max and min of red, green and blue.
    float max = (R > G) ? ((R > B) ? R : B) : ((G > B) ? G : B);
    float min = (R < G) ? ((R < B) ? R : B) : ((G < B) ? G : B);

    // Set the saturation and value
    *L = 0.5f * (max + min);

    if (max == min)
    {
        *H = 0;
        *S = 0;
    }
    else
    {
        float delta = max - min;
        
        *S = (*L <= 0.5f) ? delta / (max + min) :
                            delta / (2 - max - min);

        if (R == max)
            *H = (G - B) / delta;

        else if (G == max)
            *H = 2 + (B - R) / delta;

        else if (B == max)
            *H = 4 + (R - G) / delta;

        *H /= 6.0f;

        if (*H < 0.0f)
            *H += 1.0f;
    }
}


inline float
HLSValue(float n1, float n2, float hue)
{
    if (hue > 1.0f)
        hue -= 1.0f;
    else if (hue < 0.0f)
        hue += 1.0f;

    if (hue < 1.0f/6.0f)
        return n1 + 6 * (n2 - n1)*hue;
    else if (hue < 0.5f)
        return n2;
    else if (hue < 2.0f/3.0f)
        return n1 + 6 * (n2 - n1)* (2.0f/3.0f - hue);
    else
        return n1;
}


//! Convert a color in HLS colorspace to an equivalent color in RGB colorspace.
/*!
    This is derived from sample code in:

    Foley et al. Computer Graphics: Principles and Practice.
        Second edition in C. 592-596. July 1997.
*/
void
HLSToRGB(float *R, float *G, float *B, float H, float L, float S)
{
    if (S == 0)
    {
        // achromatic case
        *R = *G = *B = L;
    }
    else
    {
        // chromatic case
        float m1, m2;
        m2 = (L <= 0.5f) ? L* (1 + S) : L + S - L*S; 
        m1 = 2*L - m2;

        *R = HLSValue(m1, m2, H + 1.0f/3.0f);
        *G = HLSValue(m1, m2, H);
        *B = HLSValue(m1, m2, H - 1.0f/3.0f);
    }
}

} // namespace Img
