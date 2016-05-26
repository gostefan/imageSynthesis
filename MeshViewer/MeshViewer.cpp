//
//  MeshViewer.cpp
//  ImageSynthesisFramework
//
//  Created by Wojciech Jarosz on 3/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#if HAVE_CONFIG_H
#  include <config.h>
#endif // HAVE_CONFIG_H

#include "MeshViewer.h"

#include <Math/MeshBase.h>
#include <Math/Vec3.h>
#include <Math/Mat44.h>
#include <Math/MathGL.h>
#include <Math/FastMath.h>
#include <OGL/Core.h>
#include <OGL/GLUTMaster.h>
#include <OGL/GfxGLUTWindow.h>
#include <OGL/Text.h>
#include <iostream>
#include <vector>
#if !defined(_WIN32)
#include <unistd.h>
#endif
#include <iomanip>


using namespace std;
using namespace OGL;
using namespace Math;


namespace {
	const char* helpString = 
    "How to use this demo:\n"
    "\n"
    "Keyboard usage is as follows:\n"
    "    'h'     Toggle this help screen\n"
    "    'H'     Toggle heads-up-display\n"
    "    'g'     Toggle grid\n"
    "    ' '     Reset camera\n";
	
} // namespace 


MeshViewer::MeshViewer(GLUTMaster * glutMaster,
					   int width, int height,
					   const char * title,
					   MeshBase * mesh) :
		GfxGLUTWindow(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH, width, height),
		m_mouseMode(MM_NULL),
		m_mesh(mesh)
{
    glutMaster->createWindow(title, this);
	
    // Background color
    glClearColor(0.0, 0.0, 0.0, 1.0);
	
    // Antialiasing settings
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPointSize(3.0f);
	
    glPixelZoom(1.0f,-1.0f);
    
	resetView();
}


MeshViewer::~MeshViewer() {
    glutDestroyWindow(m_windowID);
}

void MeshViewer::resetView() {
	m_camera.incline = 0.0f;
	m_camera.azimuth = 0.0f;
	m_camera.distance = 2.0f;
	m_camera.center.set(0.0f);
}


void MeshViewer::update() {
    display();
}


void MeshViewer::display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// update camera matrix
    m_camera.matrix.makeIdentity();
    m_camera.matrix.setD(0, 0, -m_camera.distance);
    m_camera.matrix.rotateAxisAngle(Vec3f(1.0f, 0.0f, 0.0f), radians(m_camera.incline));
    m_camera.matrix.rotateAxisAngle(Vec3f(0.0f, 1.0f, 0.0f), radians(m_camera.azimuth));
	
	
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	
    // Set perspective projection
    gluPerspective(55, float(m_windowWidth)/float(m_windowHeight), 0.1f, 100.0f);
	
    // Place camera
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	
    // Place camera
    glMultMatrix(m_camera.matrix);
	
	
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0; i < m_mesh->numTris; ++i) {
		glVertex(m_mesh->vertices[m_mesh->vertexIndices[i].x]);
		glVertex(m_mesh->vertices[m_mesh->vertexIndices[i].y]);
		glVertex(m_mesh->vertices[m_mesh->vertexIndices[i].z]);
	}
	glEnd();
	
	drawGrid(32);
    
    char buffer[1024];
    _snprintf(buffer, 1024,
             "Press \"h\" for help\n");
    drawHUD(buffer);
    drawHelp(helpString);
    
    // Finish drawing scene
    glFinish();
    glutSwapBuffers();
}


void MeshViewer::reshape(int width, int height) {
    GfxGLUTWindow::reshape(width, height);
    display();
}


void MeshViewer::keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case ' ':
			resetView();
            update();
			break;
			
        default:
            GfxGLUTWindow::keyboard(key, x, y);
			break;
    }
	glutPostRedisplay();
}


void MeshViewer::mouse(int btn, int state, int x, int y) {
    GfxGLUTWindow::mouse(btn, state, x, y);
	
    if (btn == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) 
            m_mouseMode = MM_ROTATE;
        else
            m_mouseMode = MM_NULL;
    } else if (btn == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN)
            m_mouseMode = MM_ZOOM;
        else
            m_mouseMode = MM_NULL;
    }
}


void MeshViewer::motion(int nx, int ny) {
    int dx = nx - m_mouseX;
    int dy = ny - m_mouseY;
	
    m_mouseX = nx;
    m_mouseY = ny;
	
    switch(m_mouseMode) {
        case MM_ROTATE:
			m_camera.azimuth += dx;
			m_camera.incline += dy;
            update();
			break;
			
        case MM_ZOOM: {
			float r = 1.0f - 0.01f*dx;
			m_camera.distance *= r;
            update();
			break;
		}
			
        default:
			break;
    }
}


void MeshViewer::drawGrid(int gridRes) {
	if (!m_drawGrid)
		return;
	
	Vec2f v1;
	Vec3f v2;
	glBegin(GL_LINES);
	
	{
		int fineGridRes = 16*gridRes;
		glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
		for (int j = 0; j < fineGridRes; j++) {
			for (int i = 0; i <= gridRes; i++) {
				v1 = Vec2f((j)/(float)fineGridRes, i/(float)gridRes);
				glVertex(v2);
				
				v1 = Vec2f((j+1)/(float)fineGridRes, i/(float)gridRes);
				glVertex(v2);
			}
		}
		for (int j = 0; j <= gridRes; j++) {
			for (int i = 0; i < fineGridRes; i++) {
				v1 = Vec2f(j/(float)gridRes, i/(float)fineGridRes);
				glVertex(v2);
				
				v1 = Vec2f(j/(float)gridRes,(i+1)/(float)fineGridRes);
				glVertex(v2);
			}
		}
	}
	
	glEnd();
}
