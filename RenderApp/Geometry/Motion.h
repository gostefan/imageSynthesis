
#pragma once

#include <Math/Mat44.h>
#include "BBox.h"

using namespace Math;

class Motion {
	public:
		Motion(Vec3f translation);

		virtual Vec3f getTranslation(float t) const;

	protected:
		Vec3f completeTranslation;
};