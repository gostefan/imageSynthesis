//
//  SamplingApp.cpp
//  ImageSynthesisFramework
//
//  Created by Wojciech Jarosz on 3/6/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "SamplingApp.h"

#include "Sampling/Sampler/JitterSampler.h"
#include "Sampling/Sampler/RandomSampler.h"
#include "Sampling/Sampler/UniformSampler.h"
#include "Sampling/Warping/CosineHemisphereWarping.h"
#include "Sampling/Warping/PhongHemisphereWarping.h"
#include "Sampling/Warping/UniformCylinderWarping.h"
#include "Sampling/Warping/UniformDiskWarping.h"
#include "Sampling/Warping/UniformHemisphereWarping.h"
#include "Sampling/Warping/UniformSphereCapWarping.h"
#include "Sampling/Warping/UniformSphereWarping.h"
#include "Sampling/Warping/UniformSquareWarping.h"

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
#include <functional>


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

	auto VALUE_X_Y = [](const Math::Vec3f& point) { return pow(point.x, 2) + pow(point.y, 2)				  ; };
	auto VALUE_X   = [](const Math::Vec3f& point) { return					 pow(point.y, 2)				  ; };
	auto VALUE_Z   = [](const Math::Vec3f& point) { return									   pow(point.z, 2); };
	//double nominator = sin(warpedPoints[i].x)*sin(warpedPoints[i].y)*sin(warpedPoints[i].z);

	std::function<float(Math::Vec3f&)> VALUES[] = { VALUE_X_Y, VALUE_X, VALUE_Z };
	const char* VALUE_NAMES[] = { "x^2 + y^2", "   x^2   ", "   z^2   " };
} // namespace 


SamplingApp::SamplingApp(GLUTMaster* glutMaster, int width, int height, const char* title) :
		mSampler(new RandomSampler()),
		mWarping(new UniformSquareWarping),
		GfxGLUTWindow(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH, width, height),
		m_mouseMode(MM_NULL),
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
	
    glPixelZoom(1.0f, -1.0f);

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

/**
 * Renders to the viewport
 */
void SamplingApp::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// update camera matrix
    m_camera.matrix.makeIdentity();
    m_camera.matrix.setD(0, 0, -m_camera.distance);
    m_camera.matrix.rotateAxisAngle(Vec3f(1.0f, 0.0f, 0.0f), radians(m_camera.incline));
    m_camera.matrix.rotateAxisAngle(Vec3f(0.0f, 1.0f, 0.0f), radians(m_camera.azimuth));

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	
    // Set perspective projection
    gluPerspective(55, static_cast<float>(m_windowWidth) / static_cast<float>(m_windowHeight), 0.1f, 100.0f);
	
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
             mSampler->getName(),
             mWarping->getName(),
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
	switch (key) {
		case '1':
			mSampler.reset(new RandomSampler());
			generateSamples();
			break;
		case '2':
			mSampler.reset(new UniformSampler());
			generateSamples();
			break;
		case '3':
			mSampler.reset(new JitterSampler());
			generateSamples();
			break;

		case 'q':
			mWarping.reset(new UniformSquareWarping());
			warpSamples();
			break;
		case 'w':
			mWarping.reset(new UniformDiskWarping());
			warpSamples();
			break;
		case 'e':
			mWarping.reset(new UniformCylinderWarping());
			warpSamples();
			break;
		case 'r':
			mWarping.reset(new UniformSphereWarping());
			warpSamples();
			break;
		case 't':
			if (capValue < 200)
				capValue++;
			mWarping.reset(new UniformSphereCapWarping(capValue / 100.f));
			warpSamples();
			break;
		case 'g':
			if (capValue > 1)
				capValue--;
			mWarping.reset(new UniformSphereCapWarping(capValue / 100.f));
			warpSamples();
			break;
		case 'y':
		case 'z':
			mWarping.reset(new UniformHemisphereWarping());
			warpSamples();
			break;
		case 'u':
			mWarping.reset(new CosineHemisphereWarping());
			warpSamples();
			break;
		case 'i':
			if (nValue < 100)
				nValue++;
			mWarping.reset(new PhongHemisphereWarping(nValue / 100.f));
			warpSamples();
			break;
		case 'k':
			if (nValue > 0)
				nValue--;
			mWarping.reset(new PhongHemisphereWarping(nValue / 100.f));
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
			for (size_t valuePos = 0; valuePos < 3; valuePos++) {
				sum = 0;
				for (size_t i = 0; i < nPointsSqrt*nPointsSqrt; i++) {
					double nominator = VALUES[valuePos](warpedPoints[i]);
					double denominator = mWarping->pdf(inputPoints[i]);
					sum += nominator / denominator;
				}
				sum /= nPointsSqrt*nPointsSqrt;
				cout << "Estimated Value for function " << VALUE_NAMES[valuePos] << ": " << sum << "\n";
			}
			break;
		case 's':
			sum = 0;
			for(size_t i = 0; i < nPointsSqrt*nPointsSqrt; i++) {
				double denominator = mWarping->pdf(inputPoints[i]);
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
	mSampler->generateSamples(nPointsSqrt, inputPoints);
}

void SamplingApp::warpSamples() {
	warpedPoints.reset(new Vec3f[nPointsSqrt*nPointsSqrt]);

	for (unsigned int i = 0; i < nPointsSqrt*nPointsSqrt; i++) {
		Vec2f current = inputPoints[i];
		warpedPoints[i] = mWarping->warp(current);
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
	return mWarping->warp(Sample);
}

unsigned int SamplingApp::getNPoints() {
	return nPoints;
}

void SamplingApp::setNPoints(unsigned int nPoints_in) {
	nPoints = nPoints_in;
	nPointsSqrt = static_cast<unsigned int>(Math::round2Int(sqrt(static_cast<float>(nPoints))));
}