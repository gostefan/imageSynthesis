
#pragma once

#include "Motion.h"

#include <Math/Mat44.h>

using namespace Math;

class TranslatedMotion : public Motion {
	public:
		TranslatedMotion(Vec3f translation1, Vec3f translation2);

		virtual Vec3f getTranslation(float t) const;

	protected:
		Vec3f translation1;
};
