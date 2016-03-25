#ifndef Proc_h
#define Proc_h

class Proc
{
public:
	Proc(const MeshBase * mesh, int maxDepth, int maxObjects, float shutterTime) :
		maxDepth(maxDepth),
			maxObjects(maxObjects), motion(mesh->motion), nTime(mesh->nTime)
	{
		if (!mesh)
			return;
		
		// Compute timestep
		float timeStep = shutterTime / (nTime-1);

		// Compute bounding boxes of all triangles
		m_bboxes.reserve(mesh->numTris*nTime);

		Vec3f transl1 = Vec3f(0,0,0);
		Vec3f transl2 = Vec3f(0,0,0);
		float t1 = 0;
		float t2 = 0;
		for (int j = 1; j <= nTime; j++) {
			t1 = t2;
			t2 = j*timeStep;
			if (motion != 0) {
				transl1 = motion->getTranslation(t1);
				transl2 = motion->getTranslation(t2);
			}
			for (unsigned i = 0; i < mesh->numTris; ++i)
			{
				Box4f tempBox;
				tempBox.enclose(Vec4f(mesh->vertices[mesh->vertexIndices[i].x]+transl1, t1));
				tempBox.enclose(Vec4f(mesh->vertices[mesh->vertexIndices[i].y]+transl1, t1));
				tempBox.enclose(Vec4f(mesh->vertices[mesh->vertexIndices[i].z]+transl1, t1));
				tempBox.enclose(Vec4f(mesh->vertices[mesh->vertexIndices[i].x]+transl2, t2));
				tempBox.enclose(Vec4f(mesh->vertices[mesh->vertexIndices[i].y]+transl2, t2));
				tempBox.enclose(Vec4f(mesh->vertices[mesh->vertexIndices[i].z]+transl2, t2));
				m_bboxes.push_back(tempBox);
			}
		}
	}

	Proc(const vector<Sphere*>& drops, Motion* motion, int nTime, int maxDepth, int maxObjects, float shutterTime) :
		maxDepth(maxDepth), maxObjects(maxObjects), motion(motion), nTime(nTime)
	{
		if (drops.size() <= 0)
			return;
		
		// Compute timestep
		float timeStep = shutterTime / (nTime-1);

		// Compute bounding boxes of all triangles
		m_bboxes.reserve(drops.size()*nTime);

		Vec3f transl1 = Vec3f(0,0,0);
		Vec3f transl2 = Vec3f(0,0,0);
		float t1 = 0;
		float t2 = 0;
		for (int j = 1; j <= nTime; j++) {
			t1 = t2;
			t2 = j*timeStep;
			for (unsigned i = 0; i < drops.size(); ++i)
			{
				if (drops[i]->getMotion() != 0) {
					transl1 = drops[i]->getMotion()->getTranslation(t1);
					transl2 = drops[i]->getMotion()->getTranslation(t2);
				}
				Vec3f rad(drops[i]->getRadius(), drops[i]->getRadius(), drops[i]->getRadius());

				Box4f tempBox;
				tempBox.enclose(Vec4f(drops[i]->getPosition()+rad+transl1, t1));
				tempBox.enclose(Vec4f(drops[i]->getPosition()-rad+transl1, t1));
				tempBox.enclose(Vec4f(drops[i]->getPosition()+rad+transl2, t2));
				tempBox.enclose(Vec4f(drops[i]->getPosition()-rad+transl2, t2));
				m_bboxes.push_back(tempBox);
			}
		}
	}
	
	inline unsigned nObjs() const {return m_bboxes.size();}
	inline const Box4f & bbox(unsigned i) {return m_bboxes[i];}
	
	int nTime;
	int maxDepth;
	int maxObjects;
	const Motion* motion;

private:
	vector<Box4f> m_bboxes;
};
	
	
#endif