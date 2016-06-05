//
//  main.cpp
//  Image Synthesis Basecode
//
//  Created by Wojciech Jarosz on 10/31/11.
//  Copyright 2011 Student. All rights reserved.
//

#include "RendererApp.h"

#include <memory>

int main(int argc, char* argv[]) {
    auto glutMaster = std::unique_ptr<GLUTMaster>(new GLUTMaster(&argc, argv));
    auto window = std::unique_ptr<RendererApp>(new RendererApp(glutMaster.get(), 800, 600, "RendererApp"));
    glutMaster->disableIdleFunction();
	glutMaster->mainLoop();
	
    return 0;
}

