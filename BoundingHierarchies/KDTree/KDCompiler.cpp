
#include "BoundingHierarchies\KDTree\KDTree.h"
#include "Math\Box.h"
#include "Math\Fwd.h"

#include <vector>

class Element {
	public:
		const Math::Box<Math::Vec3f> getBBox() const {
			return Math::Box<Math::Vec3f>();
		}
};

std::vector<std::unique_ptr<Element> > b;
KDTree <Element> a(b, 10, 15);