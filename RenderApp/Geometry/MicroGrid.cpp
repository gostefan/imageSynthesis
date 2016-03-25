#include <Math/Core.h>
#include <stack>

#include "MicroGrid.h"
#include "../Scene/Camera.h"
#include "Shape.h"
#include "../Shader/Shader.h"
//#include <Img/Core.h>


using namespace Math;
//using namespace Img;

MicroGrid::MicroGrid(const Shape * shape, const SurfaceShader* shader) :
	shape(shape), m_shader(shader)
{
	w = h = 0;
}

MicroGrid::~MicroGrid()
{
	clear();
}

bool MicroGrid::allocate(short ww, short hh)
{	
	if (w != 0 || h != 0 || ww == 0 || hh == 0)
		return false;

	w = ww;
	h = hh;

	vertices = new Vec3f[w*h];
	normals = new Vec3f[w*h];
	colors = new Color3f[w*h];

	if (vertices == 0 || normals == 0 || colors == 0)
		return false;

	return true;
}

void MicroGrid::clear()
{
	if (w>0 && h>0)
	{
		delete [] normals;
		delete [] vertices;
		delete [] colors;

		w = h = 0;
	}
}

void MicroGrid::setVertex(int x, int y, const Vec3f & v)
{
	vertices[x+y*w] = v;
}

void MicroGrid::setNormal(int x, int y, const Vec3f & n)
{
	normals[x+y*w] = n;
}

void MicroGrid::setColor(int x, int y, const Color3f & c)
{
	colors[x+y*w] = c;
}


Vec3f MicroGrid::vertex(int x, int y) const
{
	return vertices[x+y*w];
}

Vec3f MicroGrid::normal(int x, int y) const
{
	return normals[x+y*w];
}

Color3f MicroGrid::color(int x, int y) const
{
	return colors[x+y*w];
}

void MicroGrid::project(const Mat44f & m)
{
	for (int u = 0; u < w; ++u)
		for (int v = 0; v < h; ++v)
			setVertex(u, v, m * vertex(u, v));
}

float MicroGrid::u(int u) const
{
	float uu = float(u)/float(w-1);
	return lerp(uStart, uEnd, uu);
}

float MicroGrid::v(int v) const
{
	float vv = float(v)/float(h-1);
	return lerp(vStart, vEnd, vv);
}

void
MicroGrid::displace(const Camera & camera)
{
	//TODO: Implement displacement mapping here
}

void
MicroGrid::shade(const Camera & camera, Scene* scene)
{
	HitInfo hit;
	for (int u = 0; u < w; ++u)
		for (int v = 0; v < h; ++v) {
			hit.P = this->vertex(u,v);
			hit.N = normal(u,v);
			hit.Ng = hit.N;
			std::stack<float> refraction = std::stack<float>();
			refraction.push(1);
			setColor(u, v, m_shader->shade(hit, scene, refraction));
		}
}

void MicroGrid::rasterize(Util::Array2D<Color4f>& rgbaBuffer, Util::Array2D<float>& zBuffer) {
	for (short u = 0; u < w-1; u++) {
		for (short v = 0; v < h-1; v++) {
			MicroPolygon poly = MicroPolygon(vertex(u, v), vertex(u+1, v), vertex(u, v+1), vertex(u+1, v+1), (normal(u, v) + normal(u+1, v) + normal(u, v+1) + normal(u+1, v+1))/4, color(u, v), color(u+1, v), color(u, v+1), color(u+1, v+1), m_shader);
			poly.rasterize(rgbaBuffer, zBuffer);
		}
	}
}

void MicroGrid::bust(std::vector<MicroPolygon*>& result) {
	for (short u = 0; u < w-1; u++) {
		for (short v = 0; v < h-1; v++) {
			result.push_back(new MicroPolygon(vertex(u, v), vertex(u+1, v), vertex(u, v+1), vertex(u+1, v+1), (normal(u, v) + normal(u+1, v) + normal(u, v+1) + normal(u+1, v+1))/4, color(u, v), color(u+1, v), color(u, v+1), color(u+1, v+1), m_shader));
		}
	}
}