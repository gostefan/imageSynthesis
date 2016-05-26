
#include "MicroGrid.h"

#include "../Scene/Camera.h"
#include "../Shader/Shader.h"

#include "Shape.h"
//#include <Img/Core.h>

#include <Math/Core.h>
#include <stack>


using namespace Math;

MicroGrid::MicroGrid(const Shape* shape, const SurfaceShader* shader) :
		shape(shape), m_shader(shader) {
	width = height = 0;
}

MicroGrid::~MicroGrid() {
	clear();
}

bool MicroGrid::allocate(short width, short height) {	
	if (this->width != 0 || this->height != 0 || width == 0 || height == 0)
		return false;

	this->width = width;
	this->height = height;

	vertices.resize(width * height);
	normals.resize(width * height);
	colors.resize(width * height);

	if (vertices.empty() || normals.empty() || colors.empty())
		return false;

	return true;
}

void MicroGrid::clear() {
	if (width > 0 && height > 0) {
		normals.clear();
		vertices.clear();
		colors.clear();

		width = height = 0;
	}
}

void MicroGrid::setVertex(int x, int y, const Vec3f & v) {
	vertices[x+y*width] = v;
}

void MicroGrid::setNormal(int x, int y, const Vec3f & n) {
	normals[x+y*width] = n;
}

void MicroGrid::setColor(int x, int y, const Color3f & c) {
	colors[x+y*width] = c;
}


Vec3f MicroGrid::getVertex(int x, int y) const {
	return vertices[x+y*width];
}

Vec3f MicroGrid::getNormal(int x, int y) const {
	return normals[x+y*width];
}

Color3f MicroGrid::getColor(int x, int y) const {
	return colors[x+y*width];
}

void MicroGrid::project(const Mat44f & m) {
	for (int u = 0; u < width; ++u)
		for (int v = 0; v < height; ++v)
			setVertex(u, v, m * getVertex(u, v));
}

float MicroGrid::u(int u) const {
	float uu = float(u)/float(width-1);
	return lerp(uStart, uEnd, uu);
}

float MicroGrid::v(int v) const {
	float vv = float(v)/float(height-1);
	return lerp(vStart, vEnd, vv);
}

void MicroGrid::displace(const Camera & camera) {
	//TODO: Implement displacement mapping here
}

void MicroGrid::shade(const Camera & camera, Scene* scene) {
	HitInfo hit;
	for (int u = 0; u < width; ++u)
		for (int v = 0; v < height; ++v) {
			hit.P = getVertex(u,v);
			hit.N = getNormal(u,v);
			hit.Ng = hit.N;
			std::stack<float> refraction = std::stack<float>();
			refraction.push(1);
			setColor(u, v, m_shader->shade(hit, scene, refraction));
		}
}

void MicroGrid::rasterize(Util::Array2D<Color4f>& rgbaBuffer, Util::Array2D<float>& zBuffer) {
	for (short u = 0; u < width-1; u++) {
		for (short v = 0; v < height-1; v++) {
			MicroPolygon poly = MicroPolygon(getVertex(u, v), getVertex(u+1, v), getVertex(u, v+1), getVertex(u+1, v+1), (getNormal(u, v) + getNormal(u+1, v) + getNormal(u, v+1) + getNormal(u+1, v+1))/4, getColor(u, v), getColor(u+1, v), getColor(u, v+1), getColor(u+1, v+1), m_shader);
			poly.rasterize(rgbaBuffer, zBuffer);
		}
	}
}

void MicroGrid::bust(std::vector<MicroPolygon*>& result) {
	for (short u = 0; u < width-1; u++) {
		for (short v = 0; v < height-1; v++) {
			result.push_back(new MicroPolygon(getVertex(u, v), getVertex(u+1, v), getVertex(u, v+1), getVertex(u+1, v+1), (getNormal(u, v) + getNormal(u+1, v) + getNormal(u, v+1) + getNormal(u+1, v+1))/4, getColor(u, v), getColor(u+1, v), getColor(u, v+1), getColor(u+1, v+1), m_shader));
		}
	}
}