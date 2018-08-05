#pragma once

#include "Geometry/BBox.h"

/**
 * The basic interface for all geometry elements.
 */
class IGeometry {
	public:
		IGeometry() = default;
		IGeometry(const IGeometry&  other) = default;
		IGeometry(		IGeometry&& other) = default;
		virtual ~IGeometry() = default;

		IGeometry& operator= (const IGeometry&  other) = default;
		IGeometry& operator= (		IGeometry&& other) = default;


};
