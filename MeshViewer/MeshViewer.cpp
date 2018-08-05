//
//  MeshViewer.cpp
//  ImageSynthesisFramework
//
//  Created by Wojciech Jarosz on 3/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "MeshViewer.h"

#include "Math/MathGL.h"
#include "Math/FastMath.h"
#include "OGL/Core.h"
#include "OGL/GLUTMaster.h"
#include "OGL/GfxGLUTWindow.h"
#include "OGL/Text.h"

#include <iomanip>
#include <iostream>
#include <vector>

#if !defined(_WIN32)
#include <unistd.h>
#endif

using namespace std;
using namespace OGL;
using namespace Math;

namespace {
	constexpr char* helpString =
R"(How to use this demo:
Left click for rotation.
Right click for zoom.

Keyboard usage is as follows:
    'h'     Toggle this help screen
    'H'     Toggle heads-up-display
    'g'     Toggle grid
    ' '     Reset camera)";

	constexpr char* hForHelp = "Press \"h\" for help\n";

	void glSetupOnce() {
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POINT_SMOOTH);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glPointSize(3.0f);
		glPixelZoom(1.0f, -1.0f);
	}
} // namespace 


MeshViewer::MeshViewer(GLUTMaster& glutMaster,
					   int width, int height,
					   const char * title,
					   const MeshBase& mesh) :
		GfxGLUTWindow(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH, width, height),
		m_mouseMode(MM_NULL),
		mesh(mesh)
{
    glutMaster.createWindow(title, this);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glSetupOnce();
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

namespace {
	void drawMesh(const Math::MeshBase& mesh) {
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_TRIANGLES);
		for (unsigned int i = 0; i < mesh.numTris; ++i) {
			glVertex(mesh.vertices[mesh.vertexIndices[i].x]);
			glVertex(mesh.vertices[mesh.vertexIndices[i].y]);
			glVertex(mesh.vertices[mesh.vertexIndices[i].z]);
		}
		glEnd();
	}
}
void MeshViewer::display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	resetCamera();
	drawMesh(mesh);	
	drawGrid(32);
    
    drawHUD(hForHelp);
    drawHelp(helpString);
    
    glFinish();
    glutSwapBuffers();
}

void MeshViewer::resetCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(55, float(m_windowWidth) / float(m_windowHeight), 0.1f, 100.0f);

	m_camera.matrix.makeIdentity();
	m_camera.matrix.setD(0, 0, -m_camera.distance);
	m_camera.matrix.rotateAxisAngle(Vec3f(1.0f, 0.0f, 0.0f), radians(m_camera.incline));
	m_camera.matrix.rotateAxisAngle(Vec3f(0.0f, 1.0f, 0.0f), radians(m_camera.azimuth));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrix(m_camera.matrix);
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
