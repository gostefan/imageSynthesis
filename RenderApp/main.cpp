//
//  main.cpp
//  Image Synthesis Basecode
//
//  Created by Wojciech Jarosz on 10/31/11.
//  Copyright 2011 Student. All rights reserved.
//

#if HAVE_CONFIG_H
#  include <config.h>
#endif // HAVE_CONFIG_H

#include "RendererApp.h"

int
main(int argc, char*argv[])
{
    GLUTMaster* glutMaster = new GLUTMaster(&argc, argv);
    RendererApp* window = new RendererApp(glutMaster, 800, 600, "RendererApp");
    glutMaster->disableIdleFunction();
	glutMaster->mainLoop();
	
    delete window;
    delete glutMaster;
	
    return 0;
}

