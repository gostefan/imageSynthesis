#include "MicroPolygon.h"

#include <algorithm>
#include <Math/Vec2.h>
#include <Math/Vec4.h>
#include <Math/Core.h>
#include "../Scene/HitInfo.h"
#include "../Shader/Shader.h"


using namespace Math;
using namespace std;

MicroPolygon::MicroPolygon()
{
}

MicroPolygon::MicroPolygon(const Vec3f & p1,
                           const Vec3f & p2,
                           const Vec3f & p3,
                           const Vec3f & p4,
                           const Vec3f & nn,
                           const Color3f & c1,
                           const Color3f & c2,
                           const Color3f & c3,
                           const Color3f & c4,
						   const SurfaceShader* shader) : m_shader(shader)
{
	v[0] = p1;
	v[1] = p2;
	v[2] = p3;
	v[3] = p4;

	if (!(v[0].z <= 0 || v[1].z <= 0 || v[2].z <= 0 || v[3].z <= 0)) {
		// I thought I had to normalize these points but I didn't
		// Recoding everything though was too time expensive
		vScaled[0] = v[0];///v[0].z;
		vScaled[1] = v[1];///v[1].z;
		vScaled[2] = v[2];///v[2].z;
		vScaled[3] = v[3];///v[3].z;

		// Calculate the bounding box
		bbMin = Vec3f(min(min(vScaled[0].x, vScaled[1].x), min(vScaled[2].x, vScaled[3].x)), min(min(vScaled[0].y, vScaled[1].y), min(vScaled[2].y, vScaled[3].y)), min(min(v[0].z, v[1].z), min(v[2].z, v[3].z)));
		bbMax = Vec3f(max(max(vScaled[0].x, vScaled[1].x), max(vScaled[2].x, vScaled[3].x)), max(max(vScaled[0].y, vScaled[1].y), max(vScaled[2].y, vScaled[3].y)), max(max(v[0].z, v[1].z), max(v[2].z, v[3].z)));
	}
	else {
		vScaled[0] = Vec3f(-1);
		vScaled[1] = Vec3f(-1);
		vScaled[2] = Vec3f(-1);
		vScaled[3] = Vec3f(-1);
		
		bbMin = Vec3f(-1);
		bbMax = Vec3f(-1);
	}
	
	color[0] = c1;
	color[1] = c2;
	color[2] = c3;
	color[3] = c4;

	normal = nn;
}

void MicroPolygon::rasterize(Util::Array2D<Color4f>& rgbaBuffer, Util::Array2D<float>& zBuffer) {
	if (v[0].z <= 0 || v[1].z <= 0 || v[2].z <= 0 || v[3].z <= 0)
		return;

	// Do flat shading
	Color3f shaded = (color[0] + color[1] + color[2] + color[3])/4;

	for (unsigned int x = static_cast<unsigned int>(max(floor(bbMin.x), 0.f)); x <= min(static_cast<unsigned int> (ceil(bbMax.x)), (unsigned int) rgbaBuffer.m_sizeX-1); x++) {
		for (unsigned int y = static_cast<unsigned int> (max(floor(bbMin.y), 0.f)); y <= min(static_cast<unsigned int> (ceil(bbMax.y)), (unsigned int) rgbaBuffer.m_sizeY-1); y++) {
			// Ignore all points with z-buffer in front of bounding box
			if (zBuffer(x,y) > bbMin.z) {

				// Calculate screen space Barycentric coordinates
				Vec3f baryFirst = calculateBarycentric2D(vScaled[0], vScaled[1], vScaled[2], Vec3f(static_cast<float> (x), static_cast<float> (y), 1));
				float zFirst = baryFirst.x * v[2].z + baryFirst.y * v[0].z + baryFirst.z * v[1].z;
				Vec3f barySecond = calculateBarycentric2D(vScaled[1], vScaled[3], vScaled[2], Vec3f(static_cast<float> (x), static_cast<float> (y), 1));
				float zSecond = barySecond.x * v[2].z + barySecond.y * v[1].z + barySecond.z * v[3].z;

				// It hit the first triangle
				if ((baryFirst.x >= 0 && baryFirst.y >= 0 && baryFirst.z >= 0 && zFirst < zBuffer(x,y))
					// It hit the second triangle
					|| (barySecond.x >= 0 && barySecond.y >= 0 && barySecond.z >= 0 && zSecond < zBuffer(x,y))) {

						// Calculate average color and point for shading
						rgbaBuffer(x,y).x = shaded.x;
						rgbaBuffer(x,y).y = shaded.y;
						rgbaBuffer(x,y).z = shaded.z;
						rgbaBuffer(x,y).w = 1;
						if (baryFirst.x >= 0 && baryFirst.y >= 0 && baryFirst.z >= 0) {
							zBuffer(x,y) = zFirst;
						}
						else {
							zBuffer(x,y) = zSecond;
						}
				}
			}
		}
	}
}

Vec3f MicroPolygon::calculateBarycentric(Vec3f c1, Vec3f c2, Vec3f c3, Vec3f point) {
	Vec3f dir1 = (c2 - c1).normalized();
	Vec3f offset1 = point - c1;
	Vec3f scaling1 = c3 - c1;
	Vec3f height1 = scaling1 - dir1*dot(dir1, scaling1);
	float bary1 = dot(offset1 - dir1*dot(dir1, offset1), scaling1 - dir1*dot(dir1, scaling1)) / height1.length2();
	Vec3f dir2 = (c3 - c2).normalized();
	Vec3f offset2 = point - c2;
	Vec3f scaling2 = c1 - c2;
	Vec3f height2 = scaling2 - dir2*dot(dir2, scaling2);
	float bary2 = dot(offset2 - dir2*dot(dir2, offset2), scaling2 - dir2*dot(dir2, scaling2)) / height2.length2();
	Vec3f dir3 = (c1 - c3).normalized();
	Vec3f offset3 = point - c3;
	Vec3f scaling3 = c2 - c3;
	Vec3f height3 = scaling3 - dir3*dot(dir3, scaling3);
	float bary3 = dot(offset3 - dir3*dot(dir3, offset3), scaling3 - dir3*dot(dir3, scaling3)) / height3.length2();

	return Vec3f(bary1,bary2,bary3);
}

Vec3f MicroPolygon::calculateBarycentric2D(Vec3f c1, Vec3f c2, Vec3f c3, Vec3f point) {
	Vec3f dir1 = (c2 - c1).normalized();
	Vec3f normal1 = Vec3f(dir1.y, -dir1.x, 0);
	float bary1 = dot(normal1, point - c1) / dot(normal1, c3 - c1);
	Vec3f dir2 = (c3 - c2).normalized();
	Vec3f normal2 = Vec3f(dir2.y, -dir2.x, 0);
	float bary2 = dot(normal2, point - c2) / dot(normal2, c1 - c2);
	Vec3f dir3 = (c1 - c3).normalized();
	Vec3f normal3 = Vec3f(dir3.y, -dir3.x, 0);
	float bary3 = dot(normal3, point - c3) / dot(normal3, c2 - c3);

	return Vec3f(bary1,bary2,bary3);
}

bool MicroPolygon::isOnScreen(int height, int width) {
	return bbMin.x < width && bbMax.x >= 0 && bbMin.y < height && bbMax.y >= 0;
}

