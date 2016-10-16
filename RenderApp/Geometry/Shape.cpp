
#include <Math/Core.h>

#include "BBox.h"
#include "Shape.h"

Shape::Shape(SurfaceShader* ss, Motion* motion, Displacement* displ) :
		surfaceShader(ss), displacement(displ), motion(motion) { }

Shape::~Shape() { }

/*Color3f Shape::getAmbient(Vec3f position) {
	return surfaceTexture->getAmbient(position);
}

Color3f Shape::getDiffuse(Vec3f position) {
	return surfaceTexture->getDiffuse(position);
}

Color3f Shape::getSpecular(Vec3f position) {
	return surfaceTexture->getSpecular(position);
}*/

Motion* Shape::getMotion() const {
	return motion;
}

bool Shape::isRenderable() const {
    return true;
}

Displacement* Shape::getDisplacement() const {
    return displacement;
}

bool Shape::intersect(Ray * r) const {
    return false;
}

bool Shape::intersects(Ray * r) const {
    return intersect(r);
}


void Shape::fillHitInfo(Ray * r) const {
	
}

void Shape::splitData(SurfacePatchPtr& child1, SurfacePatchPtr& child2, const SurfacePatch & parent, SplitDirection direction) const {
	if (direction == VSplit) {
		child1->vStart = parent.vStart;
		child1->vEnd = parent.vStart + (parent.vEnd-parent.vStart) / 2.f;
		child1->uStart = parent.uStart;
		child1->uEnd = parent.uEnd;

		child2->vStart = child1->vEnd;
		child2->vEnd = parent.vEnd;
		child2->uStart = parent.uStart;
		child2->uEnd = parent.uEnd;
	} else {	
		child1->uStart = parent.uStart;
		child1->uEnd = parent.uStart + (parent.uEnd - parent.uStart) / 2.f;
		child1->vStart = parent.vStart;
		child1->vEnd = parent.vEnd;

		child2->uStart = child1->uEnd;
		child2->uEnd = parent.uEnd;
		child2->vStart = parent.vStart;
		child2->vEnd = parent.vEnd;	
	}

	child1->generation = child2->generation = parent.generation+1;
	child1->shape = child2->shape = parent.shape;
}


void Shape::split(SurfacePatchList& results, const SurfacePatch& parent, SplitDirection splitDirection) const {
	auto child1 = util::make_unique<SurfacePatch>(this);
	auto child2 = util::make_unique<SurfacePatch>(this);
	
	splitData(child1, child2, parent, splitDirection);
	
	results.push_front(std::move(child1));
	results.push_front(std::move(child2));
}

void Shape::dice(MicroGrid& grid, const SurfacePatch & parent, short size_x, short size_y) const {
	if (!grid.allocate(size_x, size_y)) {
		printf("Failed grid allocation!\n");
		return;
	} 

	grid.shape = this;
	grid.uStart = parent.uStart;
	grid.vStart = parent.vStart;
	grid.uEnd = parent.uEnd;
	grid.vEnd = parent.vEnd;

    float du = 1 / float(size_x-1) * (parent.uEnd - parent.uStart);
    float dv = 1 / float(size_y-1) * (parent.vEnd - parent.vStart);
	
	for (int uu = 0; uu < size_x; uu++) {
		float u = parent.uStart + du*uu;   		
		for (int vv = 0; vv < size_y; vv++) {
            float v = parent.vStart + dv*vv;

			if (displacement) {
				// Calculate position information
				Vec3f point = evalP(u,v);
				Vec3f normal = evalN(u,v);
				Vec3f dirU, dirV;
				if (u < 0.999)
					dirU = evalP(u + 0.0001f, v) - point;
				else
					dirU = point - evalP(u - 0.0001f, v);

				if (v < 0.999)
					dirV = evalP(u, v + 0.0001f) - point;
				else
					dirV = point - evalP(u, v - 0.0001f);

				// Calculate displacement and normal pertubation
				float disp = displacement->getDisplacement(u,v);
				dirU = dirU.normalized() * displacement->getDerivativeU(u,v);
				dirV = dirV.normalized() * displacement->getDerivativeV(u,v);
				
				// set the values
     			grid.setVertex(uu, vv, evalP(u,v) + disp * normal);
				grid.setNormal(uu, vv, (normal + dirU + dirV).normalized());
			} else {
				grid.setVertex(uu, vv, evalP(u,v));
				grid.setNormal(uu, vv, evalN(u,v));
			}
		}
  	}
}