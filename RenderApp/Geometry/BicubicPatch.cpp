//
//  BicubicPatch.cpp
//  Image Synthesis Basecode
//
//  Created by Wojciech Jarosz on 10/30/11.
//  Copyright 2011 Student. All rights reserved.
//

#include "BicubicPatch.h"
#include <OGL/Primitive.h>
#include <Math/MathGL.h>
#include <algorithm>

using namespace Math;

// local function definitions
namespace
{
	
inline Vec3f
evaluate1D(const Vec3f& p0, const Vec3f& p1, const Vec3f& p2, const Vec3f& p3,
		   float t, float tt, float ttt, float tc, float ttc, float tttc)
{
	return ttt*p3 + 3.0f*(tt*tc*p2 + t*ttc*p1) + tttc*p0;
}

inline Vec3f
evaluated1D(const Vec3f& p0, const Vec3f& p1, const Vec3f& p2,
			float t, float tt, float tc, float ttc)
{
	return tt*p2 + 2*t*tc*p1 + ttc*p0;
}
	
} // namespace

BicubicPatch::BicubicPatch(SurfaceShader * ss) :
	Shape(ss)
{
	
}

BicubicPatch::BicubicPatch(SurfaceShader * ss, const Vec3f cvs[4][4], Displacement* displacement) :
	Shape(ss, 0, displacement)
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            m_CVs[i][j] = cvs[i][j];
}

void
BicubicPatch::renderGL() const
{
    glEnable(GL_LINE_STIPPLE);
    for (int j = 0; j < 4; j++)
    {
        glBegin(GL_LINE_STRIP);
		for (int i = 0; i < 4; i++)
			glVertex(m_CVs[i][j]);
        glEnd();
    }
    
    for (int i = 0; i < 4; i++)
    {
        glBegin(GL_LINE_STRIP);
		for (int j = 0; j < 4; j++)
			glVertex(m_CVs[i][j]);
        glEnd();
    }
    glDisable(GL_LINE_STIPPLE);
	
    
    const int num = 20;
	float invNum = 1.0f/num;
    Vec3f points[num+1][num+1];
    for (int j = 0; j <= num; j++)
    {
        for (int i = 0; i <= num; i++)
            points[i][j] = evalP(i*invNum, j*invNum);
    }
    
    for (int j = 0; j <= num; j++)
    {
        glBegin(GL_LINE_STRIP);
		for (int i = 0; i <= num; i++)
			glVertex(points[i][j]);
        glEnd();
    }
    
    for (int j = 0; j <= num; j++)
    {
        glBegin(GL_LINE_STRIP);
		for (int i = 0; i <= num; i++)
			glVertex(points[j][i]);
        glEnd();
    }
}

Vec3f BicubicPatch::evalP(float u, float v) const
{
    float tt = u*u;
    float ttt = tt*u;
    float tc = 1.0f - u;
    float ttc = tc*tc;
    float tttc = ttc*tc;
	
    Vec3f a = evaluate1D(m_CVs[0][0], m_CVs[0][1], m_CVs[0][2], m_CVs[0][3], u, tt, ttt, tc, ttc, tttc);
    Vec3f b = evaluate1D(m_CVs[1][0], m_CVs[1][1], m_CVs[1][2], m_CVs[1][3], u, tt, ttt, tc, ttc, tttc);
    Vec3f c = evaluate1D(m_CVs[2][0], m_CVs[2][1], m_CVs[2][2], m_CVs[2][3], u, tt, ttt, tc, ttc, tttc);
    Vec3f d = evaluate1D(m_CVs[3][0], m_CVs[3][1], m_CVs[3][2], m_CVs[3][3], u, tt, ttt, tc, ttc, tttc);
    
    tt = v*v;
    ttt = tt*v;
    tc = 1.0f - v;
    ttc = tc*tc;
    tttc = ttc*tc;
    
    return evaluate1D(a, b, c, d, v, tt, ttt, tc, ttc, tttc);
}

Vec3f BicubicPatch::evalN(float u, float v) const
{
	float tt = u*u;
    float ttt = tt*u;
    float tc = 1.0f - u;
    float ttc = tc*tc;
    float tttc = ttc*tc;
	
    Vec3f au = evaluate1D(m_CVs[1][0]-m_CVs[0][0],
                          m_CVs[1][1]-m_CVs[0][1],
                          m_CVs[1][2]-m_CVs[0][2],
                          m_CVs[1][3]-m_CVs[0][3], u, tt, ttt, tc, ttc, tttc);
    Vec3f bu = evaluate1D(m_CVs[2][0]-m_CVs[1][0],
                          m_CVs[2][1]-m_CVs[1][1],
                          m_CVs[2][2]-m_CVs[1][2],
                          m_CVs[2][3]-m_CVs[1][3], u, tt, ttt, tc, ttc, tttc);
    Vec3f cu = evaluate1D(m_CVs[3][0]-m_CVs[2][0],
                          m_CVs[3][1]-m_CVs[2][1],
                          m_CVs[3][2]-m_CVs[2][2],
                          m_CVs[3][3]-m_CVs[2][3], u, tt, ttt, tc, ttc, tttc);
	
    Vec3f av = evaluated1D(m_CVs[0][1]-m_CVs[0][0],
                           m_CVs[0][2]-m_CVs[0][1],
                           m_CVs[0][3]-m_CVs[0][2], u, tt, tc, ttc);
    Vec3f bv = evaluated1D(m_CVs[1][1]-m_CVs[1][0],
                           m_CVs[1][2]-m_CVs[1][1],
                           m_CVs[1][3]-m_CVs[1][2], u, tt, tc, ttc);
    Vec3f cv = evaluated1D(m_CVs[2][1]-m_CVs[2][0],
                           m_CVs[2][2]-m_CVs[2][1],
                           m_CVs[2][3]-m_CVs[2][2], u, tt, tc, ttc);
    Vec3f dv = evaluated1D(m_CVs[3][1]-m_CVs[3][0],
                           m_CVs[3][2]-m_CVs[3][1],
                           m_CVs[3][3]-m_CVs[3][2], u, tt, tc, ttc);
	
    
    tt = v*v;
    ttt = tt*v;
    tc = 1.0f - v;
    ttc = tc*tc;
    tttc = ttc*tc;
    
    Vec3f du = evaluated1D(au, bu, cu, v, tt, tc, ttc);
    Vec3f ev = evaluate1D(av, bv, cv, dv, v, tt, ttt, tc, ttc, tttc);
    
    return cross(du,ev).normalize();
}
	
BBox BicubicPatch::getBBox() const {
	// Calculate the extremal points of each arc
	Vec3f upper1(std::max(std::max(m_CVs[0][0].x, m_CVs[0][1].x), std::max(m_CVs[0][2].x, m_CVs[0][3].x)),std::max(std::max(m_CVs[0][0].y, m_CVs[0][1].y), std::max(m_CVs[0][2].y, m_CVs[0][3].y)), std::max(std::max(m_CVs[0][0].z, m_CVs[0][1].z), std::max(m_CVs[0][2].z, m_CVs[0][3].z)));
	Vec3f upper2(std::max(std::max(m_CVs[1][0].x, m_CVs[1][1].x), std::max(m_CVs[1][2].x, m_CVs[1][3].x)),std::max(std::max(m_CVs[1][0].y, m_CVs[1][1].y), std::max(m_CVs[1][2].y, m_CVs[1][3].y)), std::max(std::max(m_CVs[1][0].z, m_CVs[1][1].z), std::max(m_CVs[1][2].z, m_CVs[1][3].z)));
	Vec3f upper3(std::max(std::max(m_CVs[2][0].x, m_CVs[2][1].x), std::max(m_CVs[2][2].x, m_CVs[2][3].x)),std::max(std::max(m_CVs[2][0].y, m_CVs[2][1].y), std::max(m_CVs[2][2].y, m_CVs[2][3].y)), std::max(std::max(m_CVs[2][0].z, m_CVs[2][1].z), std::max(m_CVs[2][2].z, m_CVs[2][3].z)));
	Vec3f upper4(std::max(std::max(m_CVs[3][0].x, m_CVs[3][1].x), std::max(m_CVs[3][2].x, m_CVs[3][3].x)),std::max(std::max(m_CVs[3][0].y, m_CVs[3][1].y), std::max(m_CVs[3][2].y, m_CVs[3][3].y)), std::max(std::max(m_CVs[3][0].z, m_CVs[3][1].z), std::max(m_CVs[3][2].z, m_CVs[3][3].z)));
	Vec3f lower1(std::min(std::min(m_CVs[0][0].x, m_CVs[0][1].x), std::min(m_CVs[0][2].x, m_CVs[0][3].x)),std::min(std::min(m_CVs[0][0].y, m_CVs[0][1].y), std::min(m_CVs[0][2].y, m_CVs[0][3].y)), std::min(std::min(m_CVs[0][0].z, m_CVs[0][1].z), std::min(m_CVs[0][2].z, m_CVs[0][3].z)));
	Vec3f lower2(std::min(std::min(m_CVs[1][0].x, m_CVs[1][1].x), std::min(m_CVs[1][2].x, m_CVs[1][3].x)),std::min(std::min(m_CVs[1][0].y, m_CVs[1][1].y), std::min(m_CVs[1][2].y, m_CVs[1][3].y)), std::min(std::min(m_CVs[1][0].z, m_CVs[1][1].z), std::min(m_CVs[1][2].z, m_CVs[1][3].z)));
	Vec3f lower3(std::min(std::min(m_CVs[2][0].x, m_CVs[2][1].x), std::min(m_CVs[2][2].x, m_CVs[2][3].x)),std::min(std::min(m_CVs[2][0].y, m_CVs[2][1].y), std::min(m_CVs[2][2].y, m_CVs[2][3].y)), std::min(std::min(m_CVs[2][0].z, m_CVs[2][1].z), std::min(m_CVs[2][2].z, m_CVs[2][3].z)));
	Vec3f lower4(std::min(std::min(m_CVs[3][0].x, m_CVs[3][1].x), std::min(m_CVs[3][2].x, m_CVs[3][3].x)),std::min(std::min(m_CVs[3][0].y, m_CVs[3][1].y), std::min(m_CVs[3][2].y, m_CVs[3][3].y)), std::min(std::min(m_CVs[3][0].z, m_CVs[3][1].z), std::min(m_CVs[3][2].z, m_CVs[3][3].z)));

	// Calculate the extremal points of these arcs
	Vec3f upper(std::max(std::max(upper1.x, upper2.x), std::max(upper3.x, upper4.x)),std::max(std::max(upper1.y, upper2.y), std::max(upper3.y, upper4.y)), std::max(std::max(upper1.z, upper2.z), std::max(upper3.z, upper4.z)));
	Vec3f lower(std::min(std::min(upper1.x, upper2.x), std::min(upper3.x, upper4.x)),std::min(std::min(upper1.y, upper2.y), std::min(upper3.y, upper4.y)), std::min(std::min(upper1.z, upper2.z), std::min(upper3.z, upper4.z)));
	
	return BBox(lower, upper);
}
BBox BicubicPatch::getBBox(float uStart, float uEnd, float vStart, float vEnd) const {
	// Calculate corner points
	Vec3f point1 = evalP(uStart, vStart);
	Vec3f point2 = evalP(uStart, vEnd);
	Vec3f point3 = evalP(uEnd, vStart);
	Vec3f point4 = evalP(uEnd, vEnd);
	float sumDist1 = (point1 - point2).length2() + (point2 - point3).length2() + (point3 - point4).length2() + (point4 - point1).length2();

	Vec3f upper1(std::max(std::max(point1.x, point2.x), std::max(point3.x, point4.x)), std::max(std::max(point1.y, point2.y), std::max(point3.y, point4.y)), std::max(std::max(point1.z, point2.z), std::max(point3.z, point4.z)));
	Vec3f lower1(std::min(std::min(point1.x, point2.x), std::min(point3.x, point4.x)), std::min(std::min(point1.y, point2.y), std::min(point3.y, point4.y)), std::min(std::min(point1.z, point2.z), std::min(point3.z, point4.z)));

	// Calculate support point coefficients
    float tts = uStart*uStart;
    float tcs = 1.0f - uStart;
    float ttcs = tcs*tcs;
    float tte = uEnd*uEnd;
    float tce = 1.0f - uEnd;
    float ttce = tce*tce;
	
	// Calculate support points in first direction
    Vec3f as = evaluated1D(m_CVs[0][1], m_CVs[0][2], m_CVs[0][3], uStart, tts, tcs, ttcs);
    Vec3f bs = evaluated1D(m_CVs[1][1], m_CVs[1][2], m_CVs[1][3], uStart, tts, tcs, ttcs);
    Vec3f cs = evaluated1D(m_CVs[2][1], m_CVs[2][2], m_CVs[2][3], uStart, tts, tcs, ttcs);
    Vec3f ds = evaluated1D(m_CVs[3][1], m_CVs[3][2], m_CVs[3][3], uStart, tts, tcs, ttcs);
    Vec3f ae = evaluated1D(m_CVs[0][0], m_CVs[0][1], m_CVs[0][2], uEnd, tte, tce, ttce);
    Vec3f be = evaluated1D(m_CVs[1][0], m_CVs[1][1], m_CVs[1][2], uEnd, tte, tce, ttce);
    Vec3f ce = evaluated1D(m_CVs[2][0], m_CVs[2][1], m_CVs[2][2], uEnd, tte, tce, ttce);
    Vec3f de = evaluated1D(m_CVs[3][0], m_CVs[3][1], m_CVs[3][2], uEnd, tte, tce, ttce);
    
	// Calculate support point coefficients in other direction
    tts = vStart*vStart;
    tcs = 1.0f - vStart;
    ttcs = tcs*tcs;
    tte = vEnd*vEnd;
    tce = 1.0f - vEnd;
    ttce = tce*tce;

	// Calculate support points in other direction
	Vec3f pss = evaluated1D(bs, cs, ds, uStart, tts, tcs, ttcs);
	Vec3f pse = evaluated1D(as, bs, cs, uEnd, tte, tce, ttce);
	Vec3f pes = evaluated1D(be, ce, de, uStart, tts, tcs, ttcs);
	Vec3f pee = evaluated1D(ae, be, ce, uEnd, tte, tce, ttce);

	float sumDist2 = (pss - pse).length2() + (pse - pee).length2() + (pee - pes).length2() + (pes - pss).length2();
	Vec3f upper, lower;

	// Avoid getting huge bounding boxes if distances of construction points are much higher than of surface poinst
	if (sumDist1 / sumDist2 < 0.1) {
		upper = upper1;
		lower = lower1;
	}
	else {
		// Find max/min of these four points
		Vec3f upper2(std::max(std::max(pss.x, pse.x), std::max(pes.x, pee.x)), std::max(std::max(pss.y, pse.y), std::max(pes.y, pee.y)), std::max(std::max(pss.z, pse.z), std::max(pes.z, pee.z)));
		Vec3f lower2(std::min(std::min(pss.x, pse.x), std::min(pes.x, pee.x)), std::min(std::min(pss.y, pse.y), std::min(pes.y, pee.y)), std::min(std::min(pss.z, pse.z), std::min(pes.z, pee.z)));

		// Unite the upper and lower from above
		upper = Vec3f(std::max(upper1.x, upper2.x), std::max(upper1.y, upper2.y), std::max(upper1.z, upper2.z));
		lower = Vec3f(std::min(lower1.x, lower2.x), std::min(lower1.y, lower2.y), std::min(lower1.z, lower2.z));
	}

	// Include Displacement
	Vec3f displ(displacement != 0 ? displacement->getScale() : 0);
	return BBox(lower - displ, upper + displ);
};