//
//  main.cpp
//  Image Synthesis Basecode
//  Mesh viewer application
//
//  Created by Wojciech Jarosz on 10/31/11.
//  Copyright 2011 Student. All rights reserved.
//


#if HAVE_CONFIG_H
#  include <config.h>
#endif // HAVE_CONFIG_H

#include "MeshViewer.h"
#include <Math/Obj.h>

using namespace Math;

void
usage()
{
	printf("Wrong number of parameters:\n");
	printf("corrent usage:\n");
	printf("MeshViewer file\n");
	
	exit(1);
}

int
main(int argc, char* argv[])
{
	if (argc <= 1)
		usage();

	if (MeshBasePtr mesh = readObjMesh(argv[1]))
	{	
		OGL::GLUTMaster* glutMaster = new OGL::GLUTMaster(&argc, argv);
		MeshViewer* window = new MeshViewer(glutMaster, 512, 512, "MeshViewer", *mesh);
		glutMaster->disableIdleFunction();
		glutMaster->mainLoop();
		
		delete window;
		delete glutMaster;
		
		return 0;
	}
	
    return 1;
}

