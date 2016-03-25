#ifndef __MICROGRID
#define __MICROGRID

#include <list>
#include <Math/Vec2.h>
#include <Math/Vec3.h>
#include <Math/Color.h>
#include <Math/Fwd.h>
#include <Util/Array2D.h>

#include "MicroPolygon.h"
#include "../Scene/Camera.h"

class Scene;

using namespace Math;

class SurfaceShader;
class Shape;
class Light;

class MicroGrid
{
private:
    Math::Vec3f *vertices;
    Math::Vec3f *normals;
    Math::Color3f *colors;
	short w, h;
	const SurfaceShader* m_shader;

public:
	
	const Shape * shape;

	float uStart, uEnd, vStart, vEnd;

	MicroGrid(const Shape * shape, const SurfaceShader* shader);
	~MicroGrid();

	short width() const {return w;}
	short height() const {return h;}

	bool allocate(short ww, short hh);
	void clear();

	void setVertex(int x, int y, const Math::Vec3f & v);
	void setNormal(int x, int y, const Math::Vec3f & n);
	void setColor(int x, int y, const Math::Color3f & c);

	Math::Vec3f vertex(int x, int y) const;
	Math::Vec3f normal(int x, int y) const;
	Math::Color3f color(int x, int y) const;

	void project(const Math::Mat44f & m);
	void displace(const Camera & camera);
	void shade(const Camera & camera,  Scene* scene);
	void rasterize(Util::Array2D<Color4f>& rgbaBuffer, Util::Array2D<float>& zBuffer);

	void bust(std::vector<MicroPolygon*>& result);

	float u(int u) const;
	float v(int v) const;
};

#endif