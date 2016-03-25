//
//  main.cpp
//  Image Synthesis Basecode
//  Random sampling application
//
//  Created by Wojciech Jarosz on 10/31/11.
//  Copyright 2011 Student. All rights reserved.
//


#if HAVE_CONFIG_H
#  include <config.h>
#endif // HAVE_CONFIG_H

#include "SamplingApp.h"

int
main(int argc, char*argv[])
{
	OGL::GLUTMaster* glutMaster = new OGL::GLUTMaster(&argc, argv);
	SamplingApp* window = new SamplingApp(glutMaster, 512, 512, "SamplingApp");
    glutMaster->disableIdleFunction();
	glutMaster->mainLoop();
	
    delete window;
    delete glutMaster;
	
    return 0;
}

