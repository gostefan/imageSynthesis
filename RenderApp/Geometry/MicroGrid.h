#pragma once

#include "MicroPolygon.h"
#include "../Scene/Camera.h"

#include <Math/Vec2.h>
#include <Math/Vec3.h>
#include <Math/Color.h>
#include <Math/Fwd.h>
#include <Util/Array2D.h>

#include <list>
#include <vector>

class Scene;

using namespace Math;

class SurfaceShader;
class Shape;
class Light;

class MicroGrid {
	private:
		std::vector<Math::Vec3f>   vertices;
		std::vector<Math::Vec3f>   normals;
		std::vector<Math::Color3f> colors;
		short width, height;
		const SurfaceShader* m_shader;

	public:
		const Shape* shape;

		float uStart, uEnd, vStart, vEnd;

		MicroGrid(const Shape* shape, const SurfaceShader* shader);
		~MicroGrid();

		short getWidth() const { return width; }
		short getHeight() const { return height; }

		bool allocate(short width, short height);
		void clear();

		void setVertex(int x, int y, const Math::Vec3f& v);
		void setNormal(int x, int y, const Math::Vec3f& n);
		void setColor(int x, int y, const Math::Color3f& c);

		Math::Vec3f getVertex(int x, int y) const;
		Math::Vec3f getNormal(int x, int y) const;
		Math::Color3f getColor(int x, int y) const;

		void project(const Math::Mat44f & m);
		void displace(const Camera& camera);
		void shade(const Camera& camera,  Scene* scene);
		void rasterize(Util::Array2D<Color4f>& rgbaBuffer, Util::Array2D<float>& zBuffer);

		void bust(MicroPolygonVector& result);

		float u(int u) const;
		float v(int v) const;
};

using MicroGridPtr = std::unique_ptr<MicroGrid>;
using MicroGridVector = std::vector<MicroGridPtr>;