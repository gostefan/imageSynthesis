//
//  main.cpp
//  Image Synthesis Basecode
//
//  Created by Wojciech Jarosz on 10/31/11.
//  Copyright 2011 Student. All rights reserved.
//

#ifndef COMPAT_H
#define COMPAT_H

#ifdef WIN32
#ifndef snprintf
#define snprintf _snprintf
#endif 
#endif


#endif // COMPAT_H