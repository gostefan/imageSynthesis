/*! \file BBH.h
    \brief Contains the implementation of an abstract BBH-tree builder.
    \author Wojciech Jarosz
*/
#ifndef MATH_BBH_H_INCLUDED
#define MATH_BBH_H_INCLUDED

#include <Math/FastMath.h>
#include <Math/Box.h>
#include <Math/Vec3.h>
#include <Platform/Fwd.h>

#include <memory>
#include <vector>
#include <assert.h>

namespace Math
{

    
/*!
    A Bounding Box Hierarchy.

    This classes builds a BBH using a simple median split heuristic.
    See Peter Shiley's book for details:

        Shirley, Peter. Fundamentals of Computer Graphics. 2002.
 
	Example usage:
 
	Create a "Proc" helper class which this BBH class calls. This class must
	provide 4 key pieces of information about the bounding boxes of the objects
	to be placed in this BBH:
		* an nObjs() fuction: returns the number of objects to construct a BBH over
		* a bbox(unsigned i) function: returns the bounding-box of the i-th object
		* int maxDepth
		* int maxObjects

	Lets say you have a vector of Triangles which you want to put into your BBH,
	you could create a Proc class like this:
	
		class Proc
		{
		public:
			Proc(const vector<Triangles*> & objects, int maxDepth, int maxObjects) :
				maxDepth(maxDepth),
				maxObjects(maxObjects),
				m_nObjs(objects.size())
			{
				// Compute bounding boxes of all triangles
				m_objBBs.reserve(objects.size());
				for (unsigned i = 0; i < nObjs(); ++i)
				{
					m_objBBs.push_back(objects[i].bbox());
				}
			}
			
			inline unsigned nObjs() const {return m_objBBs.size();}
			inline const Box3f & bbox(unsigned i) {return m_objBBs[i];}
			
			int maxDepth;
			int maxObjects;
			
		protected:
			unsigned m_nObjs;
			vector<Box3f> m_objBBs;
		};
	
 
	Then, to construct a BBH over these triangles you would call something like:
		
		Proc proc(my_triangle_vector, m_maxDepth, m_maxObjects);
		BBH::BuildStats stats;
 
		BBH bbh;
		bbh.buildTree(proc, stats);
		stats.printStats();
*/
	
	
	
class BBH {
	public:
		//! Statistics gathering for BBH construction
		class BuildStats {
			private:
				int numNodes;
				int numLeaves;
				int sumObjects;
				int minObjects;
				int maxObjects;
				int sumDepth;
				int minDepth;
				int maxDepth;
				int numLeaves0;
				int numLeaves1;
				int numLeaves2;
				int numLeaves3;
				int numLeaves4;
				int numLeaves4p;
				std::unique_ptr<Platform::Progress> progress;

			public:
				BuildStats();
				~BuildStats();

				void startBuild(const char * title, int steps);
				void stepProgress(int steps);
				void finishBuild();
				void updateInner(){numNodes++;}
				void updateLeaf(int depth, int n);
				void printStats() const;
		};
    
    
		//! A Bounding Box Hierarchy node.
		struct Node {   
			void freeMemory();
			void initInterior(BuildStats & stats, const Math::Box4f& box);
			template <typename Proc>
			void initLeaf(Proc & proc, BuildStats & stats,
						  unsigned * objNums, unsigned min, unsigned max, int depth);
			bool isLeaf() const { return leaf == 1; }
			int nObjects() const { return nObjs; }
			void renderGL() const;

			float tMin, tMax;

			Math::Box4f bbox;

			unsigned leaf:1;
			unsigned nObjs:31;      // Leaf
            
			union
			{
				unsigned right;     // Interior
				unsigned  oneIndex; // Leaf w/ one object
				unsigned* indices;  // Leaf w/ multiple objects
			};
		};
    
		~BBH();

		template <typename Proc>
		void buildTree(Proc & proc, BuildStats & stats);
		void clear();
    
		std::vector<Node> nodes;
    
	private:

		template <typename Proc>
		void buildBranch(unsigned nodeNum, unsigned *objNums,
						 unsigned min, unsigned max,
						 Proc & proc, BuildStats & stats, unsigned depth, const unsigned maxDepth,
						 float totalProg);

    
		template <typename Proc>
		unsigned split(Proc & proc, unsigned *objNums,
					   unsigned min, unsigned max, float pivot, int axis);
};



template <typename Proc>
inline void
BBH::Node::initLeaf(Proc & proc, BuildStats & stats, unsigned * objNums,
                    unsigned min, unsigned max, int depth)
{
    nObjs = max - min;
    leaf = 1;

    stats.updateLeaf(depth, nObjs);

    if (nObjs == 1)
    {
        oneIndex = objNums[min]%(proc.nObjs()/proc.nTime);
        bbox = proc.bbox(objNums[min]);
    }
    else if (nObjs > 1)
    {
        indices = new unsigned [nObjs];
        for (unsigned i = min; i < max; ++i)
        {
			// The %proc.nTime yields the original object id instead of the bbox id.
            indices[i-min] = objNums[i]%proc.nTime;
            bbox.enclose(proc.bbox(objNums[i]));
        }
    }
}


template <typename Proc>
void
BBH::buildTree(Proc & proc, BuildStats & stats)
{
    clear();
        
    nodes.push_back(Node());
        
    if (proc.nObjs() == 0)
    {
        nodes.front().initLeaf(proc, stats, 0, 0, 0, 0);
        return;
    }
    if (proc.nObjs() == 1)
    {
        unsigned objNums[] = {0};
        nodes.front().initLeaf(proc, stats, objNums, 0, 1, 0);
        return;
    }

    unsigned* objNums = new unsigned[proc.nObjs()];
    for (unsigned i = 0; i < proc.nObjs(); ++i)
        objNums[i] = i;

    stats.startBuild("Constructing BBH", 100 * proc.nObjs());
    buildBranch(0, objNums, 0, proc.nObjs(), proc, stats, 0, proc.maxDepth,
                static_cast<float>(100 * proc.nObjs()));
    stats.finishBuild();
}


template <typename Proc>
unsigned
BBH::split(Proc & proc, unsigned *objNums,
           unsigned min, unsigned max, float pivot, int axis)
{
    Math::Box4f bbox;
    unsigned retVal = min;
    
    // after this loop, all objects with centroids less than pivot
    // should be towards the beginning of the list (specifically,
    // they will be prior to index retVal in the list
    for (unsigned i = min; i < max; i++)
    {
        bbox = proc.bbox(objNums[i]);
        if (0.5f*((bbox.min)[axis] +(bbox.max)[axis]) < pivot)
        {
            std::swap(objNums[i], objNums[retVal]);
            retVal++;
        }
    }

    // if the objects are very skewed, then just split them in half
    if (retVal == min || retVal == max)
        retVal = (min+max)/2;
    
    return retVal;
}


template <typename Proc>
void
BBH::buildBranch(unsigned nodeNum, unsigned *objNums,
                 unsigned min, unsigned max,
                 Proc & proc, BuildStats & stats, unsigned depth, const unsigned maxDepth,
                 float totalProg)
{
    assert(nodeNum == nodes.size()-1);

    Node& node = nodes[nodeNum];
    // Initialize leaf node if termination criteria met
    if ((max - min) <= static_cast<unsigned int>(proc.maxObjects) || depth == maxDepth)
    {
        // make leaf
        stats.stepProgress(Math::round2Int(totalProg));
        node.initLeaf(proc, stats, objNums, min, max, depth);
        return;
    }
    
    // the bounding box for this node needs to enclose all its children
    Math::Box4f box, tempBox;
	float tMin = static_cast<float>(INT_MAX), tMax = 0;
    for (unsigned i = min; i < max; i++)
    {
        tempBox = proc.bbox(objNums[i]);
        box.enclose(tempBox);
    }
    int axis = box.majorAxis();

	// If the motion is larger than the largest direction we split along the motion axis
	if (proc.motion != 0) {
		float motion = (proc.motion->getTranslation(box.max[3]) - proc.motion->getTranslation(box.min[3])).length();
		if (box.size()[axis] < motion) {
			axis = 3;
		}
	}

    // now split according to correct axis
    unsigned int mid = split(proc, objNums, min, max,
                         0.5f * (box.max[axis] + box.min[axis]), axis);
    
    // create a new boundingVolume
    float probLeft = (mid - min) / float(max - min);
    
    // these branches are "reversed" because doing so reduces 
    // intersections/ray for some reason (need to find out why)
    node.initInterior(stats, box);
    nodes.push_back(Node());
    buildBranch(nodeNum + 1, objNums, mid, max, proc, stats,
                depth+1, maxDepth, (1.0f - probLeft) * totalProg);
    
    nodes[nodeNum].right = nodes.size();
    nodes.push_back(Node());
    buildBranch(nodes[nodeNum].right, objNums, min, mid,
                proc, stats, depth+1, maxDepth, probLeft * totalProg);
}

} // namespace Math

#endif // MATH_BBH_H_INCLUDED
