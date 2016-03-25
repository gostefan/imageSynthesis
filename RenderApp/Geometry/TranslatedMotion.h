
#ifndef TranslatedMotion_h
#define TranslatedMotion_h

#include <Math/Mat44.h>
#include "Motion.h"

using namespace Math;

class TranslatedMotion : public Motion {
public:
	TranslatedMotion(Vec3f translation1, Vec3f translation2);

	virtual Vec3f getTranslation(float t) const;

protected:
	Vec3f translation1;
};

#endif