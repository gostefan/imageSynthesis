//
//  main.cpp
//  Image Synthesis Basecode
//  Mesh viewer application
//
//  Created by Wojciech Jarosz on 10/31/11.
//  Copyright 2011 Student. All rights reserved.
//


#include "MeshViewer.h"
#include "Math/Obj.h"

#include <memory>

using namespace Math;

void usage() {
	printf("Wrong number of parameters:\n");
	printf("corrent usage:\n");
	printf("MeshViewer file\n");
}

int main(int argc, char* argv[]) {
	if (argc <= 1)
		usage();
	else if (MeshBasePtr mesh = readObjMesh(argv[1])) {	
		auto glutMaster = std::make_unique<OGL::GLUTMaster>(&argc, argv);
		auto window = std::make_unique<MeshViewer>(*glutMaster, 512, 512, "MeshViewer", *mesh);
		glutMaster->disableIdleFunction();
		glutMaster->mainLoop();
	} else {
		std::cout << "Could not read obj mesh from file" << std::endl;
		return 1;
	}

	return 0;
}

