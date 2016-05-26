//
//  BBox.h
//  Image Synthesis Basecode
//
//  Created by Stefan Goetschi on 03/04/12
//  Copyright 2011 Student. All rights reserved.
//

#pragma once

#include <Math/Mat44.h>
#include <Math/Vec3.h>
#include <Math/Fwd.h>

using namespace Math;
using namespace std;

class BBox {
	private:
		const Vec3f lower;
		const Vec3f upper;
		Vec3f dividedLower;
		Vec3f dividedUpper;
	
	public:
		BBox(Vec3f lower, Vec3f upper);
	
		Vec3f getLower();
		Vec3f getUpper();
		void project(const Math::Mat44f& m);
		bool isOnScreen(int height, int width);
		float size();
};
