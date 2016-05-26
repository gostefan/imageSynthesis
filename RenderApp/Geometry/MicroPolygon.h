
#pragma once

#include <Math/Color.h>
#include <Math/Vec3.h>
#include <vector>
#include <Util/Array2D.h>

class SurfaceShader;
class Light;

class MicroPolygon {
	private:
		Math::Vec3f v[4];
		Math::Vec3f vScaled[4];
		Math::Vec3f bbMin;
		Math::Vec3f bbMax;
		Math::Vec3f normal;
		const SurfaceShader* m_shader;

	protected:
		Math::Vec3f calculateBarycentric(Math::Vec3f c1, Math::Vec3f c2, Math::Vec3f c3, Math::Vec3f point);
		Math::Vec3f calculateBarycentric2D(Math::Vec3f c1, Math::Vec3f c2, Math::Vec3f c3, Math::Vec3f point);

	public:
		Math::Color3f color[4];

		MicroPolygon();
		MicroPolygon(const Math::Vec3f& p1,
					 const Math::Vec3f& p2,
					 const Math::Vec3f& p3,
					 const Math::Vec3f& p4,
					 const Math::Vec3f& nn,
					 const Math::Color3f& c1,
					 const Math::Color3f& c2,
					 const Math::Color3f& c3,
					 const Math::Color3f& c4,
					 const SurfaceShader* shader);

		void rasterize(Util::Array2D<Math::Color4f>& rgba,
					   Util::Array2D<float>& z);

		bool isOnScreen(int height, int width);
};