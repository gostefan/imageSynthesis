
#pragma once

#include "BBox.h"
#include "Displacement/NoDisplacement.h"
#include "MicroGrid.h"
#include "../Scene/Ray.h"
#include "Texture/Texture.h"

#include <Math/Vec3.h>

#include "util/memory.h"

#include <list>

class SurfaceShader;


enum SplitDirection {
    VSplit,
    USplit
};

class SurfacePatch;
using SurfacePatchPtr = std::unique_ptr<SurfacePatch>;
using SurfacePatchCPtr = std::unique_ptr<const SurfacePatch>;
using SurfacePatchList = std::list<SurfacePatchPtr>;

class Motion;

class Shape {
	public:
		SurfaceShader* surfaceShader;

		Displacement* getDisplacement() const;
		Motion* getMotion() const;

		/*virtual Color3f getAmbient(Vec3f position);
		virtual Color3f getDiffuse(Vec3f position);
		virtual Color3f getSpecular(Vec3f position);*/
	
		Shape(SurfaceShader* ss = 0, Motion* motion = 0, Displacement* displ = 0);
		virtual ~Shape();

		virtual bool isRenderable() const;

		virtual BBox getBBox() const = 0;
		virtual BBox getBBox(float uStart, float uEnd, float vStart, float vEnd) const = 0;

		/*
			Intersect the specified primitive in local space.

			\param r ray in the object's local space
			\param primID primitive index to intersect
		*/
		virtual bool intersect(Ray* r) const;
	
		/*
			Simpler intersection function used for shadow calculations.

			\param r ray in the object's local space
			\return true iff the ray intersects the specified primitive
		*/
		virtual bool intersects(Ray* r) const;
	
		/*
			Finishes the computation of all fields in the HitInfo.

			\param r ray containing the HitInfo to complete
		*/
		virtual void fillHitInfo(Ray* r) const;
	
	
	
		virtual void renderGL() const = 0;
		virtual void split(SurfacePatchList& results,
						   const SurfacePatch& parent,
						   SplitDirection splitDirection) const;


		virtual Math::Vec3f evalP(float u, float v) const = 0;
		virtual Math::Vec3f evalN(float u, float v) const = 0;

		void dice(MicroGrid* grid, const SurfacePatch& patch, short size_x, short size_y) const;

	protected:
		virtual void splitData(SurfacePatchPtr& child1, SurfacePatchPtr& child2, const SurfacePatch& parent, SplitDirection splitDirection) const;
		Displacement* displacement;
		Motion* motion;
		//Texture* surfaceTexture;
};


class SurfacePatch {
	public:
		const Shape* shape;
		float uStart, vStart, uEnd, vEnd;
		int uDiceRate, vDiceRate;
		int generation;
	
		SurfacePatch(const Shape* shape = 0) :
				shape(shape), uStart(0.0f), vStart(0.0f), uEnd(1.0f), vEnd(1.0f), uDiceRate(1), vDiceRate(1), generation(0) { }
	
		void split(SurfacePatchList& results, SplitDirection splitDirection) const {
			shape->split(results, *this, splitDirection);
		}
	
		void dice(MicroGrid *grid, short size_x, short size_y) const {
			shape->dice(grid, *this, size_x, size_y);
		}

		BBox getBBox() const {
			return shape->getBBox(uStart, uEnd, vStart, vEnd);
		}
};