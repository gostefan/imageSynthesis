//
//  SamplingApp.cpp
//  ImageSynthesisFramework
//
//  Created by Wojciech Jarosz on 3/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#if HAVE_CONFIG_H
#  include <config.h>
#endif // HAVE_CONFIG_H

#include "SamplingApp.h"

#include <Math/Rand.h>
#include <Math/Warp.h>
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
		"    '_/+'   Decrease/Increase points by power of 2\n"
		"    '-/='   Decrease/Increase points by 1\n"
		"    '1-3'   Cycle through point sets\n"
		"    'q-i'   Cycle through warping modes\n"
		"    ' '     Reset camera\n";
	
	enum {
		RANDOM,
		UNIFORM,
		JITTERED,
		NUM_MODES
	};
	
	const char *modes[] = {
		"Random",
		"Uniform",
		"Jittered",
	};
	
	enum {
		UNIFORM_SQUARE,
		UNIFORM_DISK,
		
		UNIFORM_CYLINDER,
		
		UNIFORM_SPHERE,
		UNIFORM_SPHERE_CAP,
		
		UNIFORM_HEMISPHERE,
		COSINE_HEMISPHERE,
		PHONG_HEMISPHERE,

		NUM_WARP_MODES
	};
	
	const char* warpModes[] = {
		"Uniform Square",
		"Uniform Disk",
		
		"Uniform Cylinder",
		
		"Uniform Sphere",
		"Uniform Sphere Cap",
		
		"Uniform Hemisphere",
		"Cosine Hemisphere",
		"Phong Hemisphere",
	};
	
} // namespace 


SamplingApp::SamplingApp(GLUTMaster* glutMaster, int width, int height, const char* title) :
		GfxGLUTWindow(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH, width, height),
		m_mouseMode(MM_NULL),
		m_pointSet(RANDOM),
		m_warpMode(UNIFORM_SQUARE),
		m_drawGrid(true), nPointsSqrt(10), nPoints(100), capValue(100), nValue(50)
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
    glPointSize(3.0f);
	
    glPixelZoom(1.0f,-1.0f);

	currentWarping.reset(new UniformSquareWarping());

	inputPoints.resize(1);
	warpedPoints.reset(new Math::Vec3f[1]);

	generateSamples();
    
	resetView();
}


SamplingApp::~SamplingApp() {
    glutDestroyWindow(m_windowID);
}

void SamplingApp::resetView() {
	m_camera.incline = 0.0f;
	m_camera.azimuth = 0.0f;
	m_camera.distance = 2.0f;
	m_camera.center.set(0.0f);
}


void SamplingApp::update() {
    display();
}


void SamplingApp::display(void) {
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
	
	
	// Display the points
	glBegin(GL_POINTS);
	
	Vec3f vertex;
	for (unsigned int i = 0; i < nPointsSqrt*nPointsSqrt; i++)
		glVertex(warpedPoints[i]);

	glEnd();
	
	// Draw grid
	drawGrid(nPointsSqrt);
    
    
    char buffer[1024];
    snprintf(buffer, 1024,
             "Press \"h\" for help\n"
             "Point type: %s\n"
             "Warp mode: %s\n"
			 "Sample number: %i\n"
			 "Cap Value:%f\n",
             modes[m_pointSet],
             warpModes[m_warpMode],
			 nPoints, capValue/100.);
    drawHUD(buffer);
    drawHelp(helpString);
    
    // Finish drawing scene
    glFinish();
    glutSwapBuffers();
}


void SamplingApp::reshape(int width, int height) {
    GfxGLUTWindow::reshape(width, height);
    display();
}


void SamplingApp::keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case '1': 
			m_pointSet = RANDOM; 
			generateSamples();
			break;
        case '2': 
			m_pointSet = UNIFORM;
			generateSamples();
			break;
        case '3': 
			m_pointSet = JITTERED; 
			generateSamples();
			break;
			

        case 'q': 
			m_warpMode = UNIFORM_SQUARE;
			currentWarping.reset(new UniformSquareWarping());
			warpSamples();
			break;
        case 'w': 
			m_warpMode = UNIFORM_DISK; 
			currentWarping.reset(new UniformDiskWarping());
			warpSamples();
			break;
        case 'e': 
			m_warpMode = UNIFORM_CYLINDER;
			currentWarping.reset(new UniformCylinderWarping());
			warpSamples();
			break;
        case 'r': 
			m_warpMode = UNIFORM_SPHERE;
			currentWarping.reset(new UniformSphereWarping());
			warpSamples();
			break;
        case 't':
			m_warpMode = UNIFORM_SPHERE_CAP;
			if (capValue < 200)
				capValue++;
			currentWarping.reset(new UniformSphereCapWarping(capValue / 100.f));
			warpSamples();
			break;
        case 'g':
			m_warpMode = UNIFORM_SPHERE_CAP; 
			if (capValue > 1)
				capValue--;
			currentWarping.reset(new UniformSphereCapWarping(capValue / 100.f));
			warpSamples();
			break;
        case 'y':
		case 'z':
			m_warpMode = UNIFORM_HEMISPHERE;
			currentWarping.reset(new UniformHemisphereWarping());
			warpSamples();
			break;
        case 'u': 
			m_warpMode = COSINE_HEMISPHERE;
			currentWarping.reset(new CosineHemisphereWarping());
			warpSamples();
			break;
        case 'i': 
			m_warpMode = PHONG_HEMISPHERE; 
			if (nValue < 100)
				nValue++;
			currentWarping.reset(new PhongHemisphereWarping(nValue / 100.f));
			warpSamples();
			break;
        case 'k': 
			m_warpMode = PHONG_HEMISPHERE; 
			if (nValue > 0)
				nValue--;
			currentWarping.reset(new PhongHemisphereWarping(nValue / 100.f));
			warpSamples();
			break;
			


        case 'G': m_drawGrid = !m_drawGrid;
            update();
            break;

        case ' ':
			resetView();
            update();
			break;

		case '*':
			setNPoints(nPoints * 2);
			generateSamples();
			break;

		case '/':
			if (nPoints > 1) {
				setNPoints(nPoints / 2);
				generateSamples();
			}
			break;

		case '+':
			setNPoints(nPoints + 1);
			generateSamples();
			break;

		case '-':
			if (nPoints > 1) {
				setNPoints(nPoints - 1);
				generateSamples();
			}
			break;

		case 'm':
			double sum;
			sum = 0;
			for(size_t i = 0; i < nPointsSqrt*nPointsSqrt; i++) {
				double nominator = 0;
				switch (m_warpMode) {
					case UNIFORM_SQUARE: 
						nominator = pow(warpedPoints[i].x, 2) + pow(warpedPoints[i].y, 2);
						break;
					case UNIFORM_DISK: 
						nominator = pow(warpedPoints[i].x, 2);
						break;
					case UNIFORM_SPHERE:
					case UNIFORM_HEMISPHERE:
					case COSINE_HEMISPHERE: 
					case UNIFORM_CYLINDER:
					case UNIFORM_SPHERE_CAP:
					case PHONG_HEMISPHERE:
						nominator = pow(warpedPoints[i].z, 2);
						break;
				}
				//double nominator = sin(warpedPoints[i].x)*sin(warpedPoints[i].y)*sin(warpedPoints[i].z);
				double denominator = currentWarping->pdf(inputPoints[i]);
				sum += nominator/denominator;
			}
			sum /= nPointsSqrt*nPointsSqrt;
			cout << "Estimated Value: " << sum << "\n";
			break;
		case 's':
			sum = 0;
			for(size_t i = 0; i < nPointsSqrt*nPointsSqrt; i++) {
				double denominator = currentWarping->pdf(inputPoints[i]);
				sum += 1/denominator;
			}
			sum /= nPointsSqrt*nPointsSqrt;
			cout << "Estimated Value: " << sum << "\n";
			break;
			
        default:
            GfxGLUTWindow::keyboard(key, x, y);
			break;
    }
	
	glutPostRedisplay();
}

void SamplingApp::generateSamples() {
	drawSamples();
	warpSamples();
}

void SamplingApp::drawSamples() {
	inputPoints.resize(nPointsSqrt*nPointsSqrt);
	
	Sampler* currentSampler;
	switch (m_pointSet) {
		case RANDOM:
			currentSampler = &RandomSampler();
			break;
		default:
		case UNIFORM:
			currentSampler = &UniformSampler();
			break;
		case JITTERED:
			currentSampler = &JitterSampler();
			break;
	}

	currentSampler->generateSamples(nPointsSqrt, inputPoints);
}

void SamplingApp::warpSamples() {
	warpedPoints.reset(new Vec3f[nPointsSqrt*nPointsSqrt]);

	for (unsigned int i = 0; i < nPointsSqrt*nPointsSqrt; i++) {
		Vec2f current = inputPoints[i];
		warpedPoints[i] = currentWarping->warp(current);
	}
}

void SamplingApp::mouse(int btn, int state, int x, int y) {
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


void SamplingApp::motion(int nx, int ny) {
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


void SamplingApp::drawGrid(int gridRes) {
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
				v2 = warpPoint(v1);
				glVertex(v2);
				
				v1 = Vec2f((j+1)/(float)fineGridRes, i/(float)gridRes);
				v2 = warpPoint(v1);
				glVertex(v2);
			}
		}
		for (int j = 0; j <= gridRes; j++) {
			for (int i = 0; i < fineGridRes; i++) {
				v1 = Vec2f(j/(float)gridRes, i/(float)fineGridRes);
				v2 = warpPoint(v1);
				glVertex(v2);
				
				v1 = Vec2f(j/(float)gridRes,(i+1)/(float)fineGridRes);
				v2 = warpPoint(v1);
				glVertex(v2);
			}
		}
	}
	
	glEnd();
}


Math::Vec3f SamplingApp::warpPoint(Math::Vec2f Sample) {
	return currentWarping->warp(Sample);
}

unsigned int SamplingApp::getNPoints() {
	return nPoints;
}

void SamplingApp::setNPoints(unsigned int nPoints_in) {
	nPoints = nPoints_in;
	nPointsSqrt = static_cast<unsigned int>(Math::round2Int(sqrt(static_cast<float>(nPoints))));
}