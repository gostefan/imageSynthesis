#include "TreeTests.h"

#include "BoundingHierarchies/KDTree/KDTree.h"

#include "Math\Box.h"
#include "Math\Vec3.h"

#include <memory>

namespace test {
	class SizedObject;
	using SizedObjectPtr = std::unique_ptr<SizedObject>;
	class SizedObject {
		public:
			SizedObject(Math::Box<Math::Vec3f> bBox) {
				mBBox = bBox;
			}
			const Math::Box<Math::Vec3f> getBBox() const { return mBBox; }
		private:
			Math::Box<Math::Vec3f> mBBox;
	};
}

namespace {
	auto createSizedObject = [](float x1, float y1, float z1, float x2, float y2, float z2) {
		return std::make_unique<test::SizedObject>(Math::Box<Math::Vec3f>(Math::Vec3f(x1, y1, z1), Math::Vec3f(x2, y2, z2)));
	};
}

namespace test {
	TestResult TreeTests::allTests() {
		TestResult result = trivialSplits();
		return result;
	}

	TestResult TreeTests::trivialSplits()
	{
		std::vector<SizedObjectPtr> elements;
		elements.push_back(createSizedObject(0, 0, 0, 1, 1, 1));
		elements.push_back(createSizedObject(1, 1, 1, 2, 2, 2));
		elements.push_back(createSizedObject(2, 2, 2, 3, 3, 3));
		KDTree<SizedObject> tree(elements, 1, 2);

		return TestResult(0,0,1);
	}
}
