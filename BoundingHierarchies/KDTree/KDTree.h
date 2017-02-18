#pragma once

#include "Math\Box.h"
#include "Math\Vec3.h"
#include "util/memory.h"

#include <list>
#include <iterator>
#include <vector>


//#define KD_DEBUG

namespace {
	template <class E>
	class KDNode;
	template <class E>
	using KDNodeArray = std::unique_ptr<KDNode<E>[]>;

	template <class E>
	class KDNode {
		public:
			union {
				uint32_t flags; // Is this necessary in nodes or do we just store the first dimension we work on?
				uint32_t nElements;
			};
			union {
				float splitPoint;
				const E* const * nodeElements;
			};
#ifdef KD_DEBUG
			uint32_t leftChildren = 0;
			uint32_t rightChilrden = 0;
#endif
	};
}

template <class E>
class KDTree {
	public:
		KDTree(std::vector<std::unique_ptr<E> >& elements, uint32_t avgElemsPerLeaf, uint32_t maxElemsPerLeaf) {
			maxElementsPerLeaf = maxElemsPerLeaf;

			const float	binaryDepth		= std::log2f(static_cast<float>(elements.size()));
			const float	leafFillDepth	= std::log2f(static_cast<float>(avgElemsPerLeaf));
						maxTreeDepth	= static_cast<uint32_t>(std::ceilf(binaryDepth - leafFillDepth));
			nodes.reset(new KDNode<E>[static_cast<uint32_t>(std::pow(2, maxTreeDepth) - 1)]);

			std::unique_ptr<E*[]> elemPtr = std::unique_ptr<E*[]>(new E*[elements.size()]);
			std::transform(elements.cbegin(), elements.cend(), stdext::checked_array_iterator<E**>(elemPtr.get(), elements.size()), [](const std::unique_ptr<E>& element) { return element.get(); });
			splitElements(nodes.get(), elemPtr.get(), elements.size(), maxTreeDepth);
			ptrStorage = std::move(elements);
		}
		virtual ~KDTree() {}

	protected:
		/**
		 * This trivial implementation just returns the midpoint of the boundingbox of all elements
		 * TODO: extract this to something like a ISplitter interface that handles this.
		 */
		virtual float getSplitPoint(const E* const * const elements, const uint32_t nElements, const uint8_t dimension) {
			Math::Box<Math::Vec3f> allBox{ };
			for (uint32_t i = 0; i < nElements; i++)
				allBox.enclose(elements[i]->getBBox());
			Math::Vec3f center = allBox.center();
			return center[dimension];
		}

		void splitElements(KDNode<E>* node, const E* const * const elements, const uint32_t nElements, const uint32_t subDepth) {
			if (subDepth <= 1 || nElements <= maxElementsPerLeaf) {
				// This is an inner node
				node->nodeElements = elements;
				node->nElements = (nElements << 2) | 3;
			} else {
				// This is a leaf
				node->flags = subDepth % 3;
				node->splitPoint = getSplitPoint(elements, nElements, node->flags);
				std::vector<const E*> leftElements;
				std::vector<const E*> rightElements;
				for (uint32_t i = 0; i < nElements; i++) {
					const Math::Box<Math::Vec3f>& box = elements[i]->getBBox();
					if (box.min[node->flags] < node->splitPoint) {
						leftElements.push_back(elements[i]);
#ifdef KD_DEBUG
						node->leftChildren++;
#endif
					}
					if (box.max[node->flags] > node->splitPoint) {
						rightElements.push_back(elements[i]);
#ifdef KD_DEBUG
						node->rightChilrden++;
#endif
					}
				}

				KDNode<E>* leftNode = node + 1;
				KDNode<E>* rightNode = node + static_cast<uint32_t>(std::pow(2, subDepth - 1));
				splitElements(leftNode,		leftElements.data(),	leftElements.size(),	subDepth - 1);
				splitElements(rightNode,	rightElements.data(),	rightElements.size(),	subDepth - 1);
			}
		}

	protected:
		uint32_t							maxTreeDepth;
		uint32_t							maxElementsPerLeaf;
		KDNodeArray<E>						nodes;
		std::vector<std::unique_ptr<E> >	ptrStorage;
};