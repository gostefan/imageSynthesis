/*! \file RendererApp.cpp
    \brief Implements the Rendering Engine
    \author Wojciech Jarosz
*/

#include "RendererApp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <Math/Color.h>
#include <Math/Core.h>
#include <Platform/Stopwatch.h>
#include <vector>

#include "Scene/Camera.h"
#include "Scene/ShutterCamera.h"
#include "Scene/DOFCamera.h"
#include "Geometry/BicubicPatch.h"
#include "Geometry/Motion.h"
#include "Geometry/Rain.h"
#include "Geometry/Shape.h"
#include "Geometry/Sphere.h"
#include "Geometry/TranslatedMotion.h"
#include "Geometry/Texture/CheckerboardTexture.h"
#include "Geometry/Texture/ImageTexture.h"
#include "Renderer/OpenGLRenderer.h"
#include "Renderer/RasterizationRenderer.h"
#include "Renderer/MultipleRayTracingRenderer.h"
#include "Renderer/RayTracingRenderer.h"
#include "Renderer/ReyesRenderer.h"
#include "Renderer/ReyesTracingRenderer.h"
#include "Renderer/PhotonScatterer.h"
#include "Scene/EnvironmentMap.h"
#include "Scene/photonmap.h"
#include "Scene/PointLight.h"
#include "Scene/UniformSphereLight.h"
#include "Scene/RectangularLight.h"
#include "Scene/SphereCapLight.h"
#include "Shader/BrushedShader.h"
#include "Shader/AmbientOcclusionShader.h"
#include "Shader/LambertShader.h"
#include "Shader/MirrorShader.h"
#include "Shader/PathTracingShader.h"
#include "Shader/PhongShader.h"
#include "Shader/PhongLobeShader.h"
#include "Shader/RefractionShader.h"


using namespace OGL;
using namespace Util;
using namespace Math;
using namespace Platform;
using namespace std;

namespace {
	const char* renderModeNames[] = {
		"RENDER_GL",
		"RENDER_REYES",
		"RENDER_RASTERIZE",
		"RENDER_RAYTRACE",
		"RENDER_PATH",
		"RENDER_REYES_TRACE"
	};


	const char* helpString =
		"Keyboard/mouse usage is as follows:\n"
		"    'h'     Toggle this help screen\n"
		"    'H'     Toggle heads-up-display\n"
		"    LMB     Rotate camera\n"
		"    MMB     Pan camera\n"
		"    RMB     Dolly camera\n"
		"    'w/s'   Dolly camera forward/backward \n"
		"    'a/d'   Pan camera left/right\n"
		"    'q/z'   Pan camera up/down\n"
		"    'g'     Switch to OpenGL rendering mode\n"
		"    'r'     Switch to REYES rendering mode\n"
		"    'x'     Switch to REYES Tracing rendering mode\n"
		"    'R'     Switch to software rasterization mode\n"
		"    't'     Switch to ray tracing mode\n"
		"    'p'     Switch to path tracing mode\n"
		"    'i'     Save HDR Image\n";

	const float g_angleFact = 0.2f;
	const float g_scaleFact = 0.002f;
	
	void createStillScene(Scene& scene) {
		scene.camera.reset(new Camera(scene.camera->xRes(), scene.camera->yRes()));

		Math::Mat44f w2c = scene.camera->worldToCamera();
		w2c.lookAt(Vec3f(0, 0, -5), Vec3f(0, 0, 1), Vec3f(0.0f, 1.0f, 0.0f));
		scene.camera->setWorldToCamera(w2c);

		scene.addLight(new PointLight(Vec3f(0, 2, 0), Color3f(50, 50, 50)));

		SurfaceShader* blue = new LambertShader(Color3f(0, 0, 1));
		SurfaceShader* red = new LambertShader(Color3f(1, 0, 0));
		scene.shapes.push_back(new Sphere(blue, Vec3f(0.5f, -0.6f, 0.5f), 0.4f));
		scene.shapes.push_back(new Mesh(red, readObjMesh("./obj/planeX2.obj")));
	}

	void createMovingScene(Scene& scene) {
		float shutterTime = 0.5f;

		scene.camera.reset(new ShutterCamera(shutterTime, std::move(scene.camera)));

		Math::Mat44f w2c = scene.camera->worldToCamera();
		w2c.lookAt(Vec3f(0, 0, -5), Vec3f(0, 0, 1), Vec3f(0.0f, 1.0f, 0.0f));
		scene.camera->setWorldToCamera(w2c);

		scene.addLight(new PointLight(Vec3f(0, 0, 0), Color3f(50, 50, 50)));
		scene.addLight(new PointLight(Vec3f(1, 0, 5), Color3f(50, 50, 50)));

		SurfaceShader * grey = new LambertShader(Color3f(1, 1, 1));
		SurfaceShader * mirror = new MirrorShader(Color3f(0.2f, 0.2f, 0.2f), 0.8f);
		SurfaceShader * glass = new RefractionShader(1.6f, Color3f(0.2f, 0.2f, 0.2f), 0.8f);

		Motion* left = new TranslatedMotion(Vec3f(1, 0, 5), Vec3f(-0.5f, 0, 0));
		Motion* offsetLeft = new TranslatedMotion(Vec3f(-1, 0.5f, 0), Vec3f(-0.5f, -0.5f, 0));
		Motion* none = new TranslatedMotion(Vec3f(-0.5, 0.5, 5), Vec3f(0, 0, 0));

		scene.shapes.push_back(new Sphere(grey, Vec3f(0, 0, 0), 0.4f, offsetLeft));
		scene.shapes.push_back(new Sphere(glass, Vec3f(0.9f, 0, 0), 0.4f));
		scene.shapes.push_back(new Mesh(grey, readObjMesh("./obj/planeX2.obj"), left, shutterTime, 2));
		scene.shapes.push_back(new Mesh(mirror, readObjMesh("./obj/planeZ.obj"), none, shutterTime, 2));
	}

	void createMovingScene2(Scene& scene) {
		float shutterTime = 0.5f;

		scene.camera.reset(new ShutterCamera(shutterTime, std::move(scene.camera)));

		Math::Mat44f w2c = scene.camera->worldToCamera();
		w2c.lookAt(Vec3f(0, 0, -10), Vec3f(0, 0, 1), Vec3f(0.0f, 1.0f, 0.0f));
		scene.camera->setWorldToCamera(w2c);

		SurfaceShader * aos = new AmbientOcclusionShader(Color3f(1, 1, 1), 10);
		Motion* offsetLeft = new TranslatedMotion(Vec3f(0.5f, 0, 0), Vec3f(-1, 0, 0));

		scene.shapes.push_back(new Sphere(aos, Vec3f(0, -30, 0), 29));
		scene.shapes.push_back(new Sphere(aos, Vec3f(0, 0, 0), 1, offsetLeft));
	}

	void createRainScene(Scene& scene) {
		float shutterTime = 0.01f;

		scene.camera.reset(new ShutterCamera(shutterTime, std::move(scene.camera)));

		Math::Mat44f w2c = scene.camera->worldToCamera();
		w2c.lookAt(Vec3f(0, 0, -10), Vec3f(0, 0, 1), Vec3f(0.0f, 1.0f, 0.0f));
		scene.camera->setWorldToCamera(w2c);

		scene.addLight(new PointLight(Vec3f(0, 0, 0), Color3f(10, 10, 10)));

		scene.shapes.push_back(new Rain(Box3f(Vec3f(-3, -1, -5), Vec3f(3, 3, 5)), 10, 50, shutterTime, 2));
	}

	void createTextureScene(Scene& scene) {
		scene.camera.reset(new Camera(scene.camera->xRes(), scene.camera->yRes()));

		Math::Mat44f w2c = scene.camera->worldToCamera();
		w2c.lookAt(Vec3f(0, 0, -10), Vec3f(0, 0, 1), Vec3f(0.0f, 1.0f, 0.0f));
		scene.camera->setWorldToCamera(w2c);

		scene.addLight(new PointLight(Vec3f(0, 0, 0), Color3f(10, 10, 10)));

		SurfaceShader* metalPlate = new LambertShader((Texture*)new ImageTexture("./imgs/metalplate.hdr"));

		scene.shapes.push_back(new Mesh(metalPlate, readObjMesh("./obj/planeZ.obj")));
	}

	void createDOFScene(Scene& scene) {
		scene.camera.reset(new DOFCamera(scene.camera->xRes(), scene.camera->yRes(), 13, 3000));

		Math::Mat44f w2c = scene.camera->worldToCamera();
		w2c.lookAt(Vec3f(0, 0, -10), Vec3f(0, 0, 1), Vec3f(0.0f, 1.0f, 0.0f));
		scene.camera->setWorldToCamera(w2c);

		scene.addLight(new PointLight(Vec3f(0, 3, 3), Color3f(50, 50, 50)));
		scene.addLight(new PointLight(Vec3f(0, 3, -1), Color3f(50, 50, 50)));

		SurfaceShader* blue = new LambertShader(Color3f(0, 0, 1));
		SurfaceShader* green = new LambertShader(Color3f(0, 1, 0));
		SurfaceShader* red = new LambertShader(Color3f(1, 0, 0));

		scene.shapes.push_back(new Sphere(blue, Vec3f(-1, 0, 1)));
		scene.shapes.push_back(new Sphere(green, Vec3f(0, 0, 3)));
		scene.shapes.push_back(new Sphere(red, Vec3f(1, 0, 5)));
	}

	void createBrushedScene(Scene& scene) {
		scene.camera.reset(new Camera(scene.camera->xRes(), scene.camera->yRes()));

		Math::Mat44f w2c = scene.camera->worldToCamera();
		w2c.lookAt(Vec3f(0, 0, -10), Vec3f(0, 0, 1), Vec3f(0.0f, 1.0f, 0.0f));
		scene.camera->setWorldToCamera(w2c);

		scene.addLight(new RectangularLight(Color3f(10, 10, 10), 0, Vec3f(-0.5f, 10, 0.5f), Vec2f(1, 1)));

		SurfaceShader* mirror = new MirrorShader(Color3f(0.8f, 0.8f, 0.8f), 0.8f, 2);
		SurfaceShader* brushedMetal1 = new BrushedShader(Color3f(0.8f, 0.8f, 0.8f), 0.8f, 2, 0, 100000, 1000);
		SurfaceShader* brushedMetal2 = new BrushedShader(Color3f(0.8f, 0.8f, 0.8f), 0.8f, 2, 0, 1000, 100000);

		scene.shapes.push_back(new Sphere(brushedMetal1, Vec3f(0, 0, 0), 0.7f));
		scene.shapes.push_back(new Sphere(brushedMetal2, Vec3f(1.8f, 0, 0), 0.7f));
		scene.shapes.push_back(new Sphere(mirror, Vec3f(-1.8f, 0, 0), 0.7f));
	}

	void createImpSampScene(Scene& scene) {
		scene.camera.reset(new Camera(scene.camera->xRes(), scene.camera->yRes()));

		Math::Mat44f w2c = scene.camera->worldToCamera();
		w2c.lookAt(Vec3f(0, 0, -10), Vec3f(0, 0, 1), Vec3f(0.0f, 1.0f, 0.0f));
		scene.camera->setWorldToCamera(w2c);

		scene.addLight(new PointLight(Vec3f(0, 0, 0), Color3f(50, 50, 50)));

		SurfaceShader * aos = new AmbientOcclusionShader(Color3f(1, 1, 1), 10, false, true);

		scene.shapes.push_back(new Sphere(aos, Vec3f(0, -30, 0), 29));
		scene.shapes.push_back(new Sphere(aos, Vec3f(0, 0, 0), 1));
	}

	void createNormalScene(Scene& scene) {
		scene.camera.reset(new Camera(scene.camera->xRes(), scene.camera->yRes()));

		Math::Mat44f w2c = scene.camera->worldToCamera();
		w2c.lookAt(Vec3f(-2000, 0, 0), Vec3f(0, 0, -20), Vec3f(0.0f, 1.0f, 0.0f));
		scene.camera->setWorldToCamera(w2c);

		scene.addLight(new PointLight(Vec3f(-300, 300, 0), Color3f(1000000, 1000000, 1000000)));

		SurfaceShader * red = new LambertShader(Color3f(1, 0, 0));
		scene.shapes.push_back(new Mesh(red, readObjMesh("./obj/mitsubishi-car.obj")));
	}


	void createInterestingScene(Scene& scene) {
		float shutterTime = 0.005f;
		float scale = 1000;

		scene.camera.reset(new ShutterCamera(shutterTime, std::unique_ptr<Camera>(new DOFCamera(scene.camera->xRes(), scene.camera->yRes(), 2000, 50000))));

		//delete scene.background;
		//scene.background = new EnvironmentMap("./imgs/meadow2.hdr");

		Math::Mat44f w2c = scene.camera->worldToCamera();
		w2c.lookAt(Vec3f(2000, 0, 0), Vec3f(0, 0, 1), Vec3f(0.0f, 1.0f, 0.0f));
		//w2c.lookAt(Vec3f(0, 1000, 0), Vec3f(0,0,1), Vec3f(0.0f, 1.0f, 0.0f));
		scene.camera->setWorldToCamera(w2c);

		scene.shapes.clear();

		scene.addLight(new SphereCapLight(Color3f(25000000, 20000000, 5000000), 0, Vec3f(-290, 138, -29), 15, 1));
		scene.addLight(new SphereCapLight(Color3f(25000000, 20000000, 5000000), 0, Vec3f(-290, 138, -879), 15, 1));
		scene.addLight(new SphereCapLight(Color3f(25000000, 20000000, 5000000), 0, Vec3f(-290, 138, 841), 15, 1));
		scene.addLight(new PointLight(Vec3f(0, 700, -800), Color3f(10000000, 10000000, 50000000)));
		scene.addLight(new PointLight(Vec3f(0, 700, 200), Color3f(10000000, 10000000, 50000000)));

		Motion* car = new TranslatedMotion(Vec3f(0, -200, 800), Vec3f(0, 0, 15 * scale)); // 15 m/s
		Motion* carOpp = new TranslatedMotion(Vec3f(500, -200, -800), Vec3f(0, 0, -15 * scale));
		Motion* sceneDown = new TranslatedMotion(Vec3f(0, -220, 0), Vec3f(0, 0, 0));

		SurfaceShader* red = new PathTracingShader(new LambertShader(Color3f(1, 0, 0)), Color3f(1, 0, 0));
		SurfaceShader* darkGrey = new PathTracingShader(new LambertShader(Color3f(0.2f, 0.2f, 0.2f)), Color3f(0.2f, 0.2f, 0.2f));
		SurfaceShader* streetGrey = new PathTracingShader(new LambertShader(Color3f(0.1f, 0.1f, 0.2f)), Color3f(0.1f, 0.1f, 0.2f));
		SurfaceShader* mirror = new MirrorShader(Color3f(0.2f, 0.2f, 0.2f), 0.5f);
		SurfaceShader* grey = new PathTracingShader(new LambertShader(Color3f(0.8f, 0.8f, 0)), Color3f(0.8f, 0.8f, 0));
		SurfaceShader* brushedMetal = new BrushedShader(Color3f(0.8f, 0.8f, 0.8f), 0.8f, 5, grey, 100, 1000, 1);
		SurfaceShader* green = new PathTracingShader(new LambertShader(Color3f(0, 0.2f, 0)), Color3f(0, 0.2f, 0));

		scene.shapes.push_back(new Mesh(red, readObjMesh("./obj/mitsubishi-car.obj"), car, shutterTime, 5));
		scene.shapes.push_back(new Mesh(mirror, readObjMesh("./obj/mitsubishi-wheel.obj"), car, shutterTime, 5));
		scene.shapes.push_back(new Mesh(darkGrey, readObjMesh("./obj/mitsubishi-pneu.obj"), car, shutterTime, 5));
		scene.shapes.push_back(new Mesh(mirror, readObjMesh("./obj/mitsubishi-light-interior.obj"), car, shutterTime, 5));
		scene.shapes.push_back(new Mesh(red, readObjMesh("./obj/mitsubishi-car-opp.obj"), carOpp, shutterTime, 5));
		scene.shapes.push_back(new Mesh(mirror, readObjMesh("./obj/mitsubishi-wheel-opp.obj"), carOpp, shutterTime, 5));
		scene.shapes.push_back(new Mesh(darkGrey, readObjMesh("./obj/mitsubishi-pneu-opp.obj"), carOpp, shutterTime, 5));
		scene.shapes.push_back(new Mesh(mirror, readObjMesh("./obj/mitsubishi-light-interior-opp.obj"), carOpp, shutterTime, 5));

		scene.shapes.push_back(new Mesh(brushedMetal, readObjMesh("./obj/pole.obj"), sceneDown, shutterTime, 2));
		scene.shapes.push_back(new Mesh(streetGrey, readObjMesh("./obj/street.obj"), sceneDown, shutterTime, 2));

		scene.shapes.push_back(new Rain(Box3f(Vec3f(-1000, -200, -1000), Vec3f(1000, 500, 1000)), scale, 1000, shutterTime, 10));

		scene.shapes.push_back(new Sphere(green, Vec3f(0, -15000, 0), 14600));
	}
}


RendererApp::RendererApp(GLUTMaster* glutMaster, int width, int height, const char* title) :
		GfxGLUTWindow(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH, width, height), mRenderMode(RENDER_GL), nSamplesSqrt(4), sceneNr(1)
{
	m_scene.camera->setResolution(width, height);
	
    glutMaster->createWindow(title, this);
    glutMaster->setIdleToCurrentWindow();
    setCurrentFont(0);
	
	glClearColor(0, 0, 0, 1);
	glDisable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	createScene(sceneNr);
}


void RendererApp::createScene(int number) {
	sceneNr = number;
	m_scene.shapes.clear();
	m_scene.lights.clear();
	switch (number) {
		default:
		case 1:
			createStillScene(m_scene);
			break;
		case 2:
			createMovingScene(m_scene);
			break;
		case 3:
			createMovingScene2(m_scene);
			break;
		case 4:
			createRainScene(m_scene);
			break;
		case 5:
			createTextureScene(m_scene);
			break;
		case 6:
			createBrushedScene(m_scene);
			break;
		case 7:
			createDOFScene(m_scene);
			break;
		case 8:
			createImpSampScene(m_scene);
			break;
		case 9:
			createNormalScene(m_scene);
			break;
		case 0:
			createInterestingScene(m_scene);
			break;
		case 10:
			break;
	}
}

RendererApp::~RendererApp() { }

void RendererApp::reshape(int width, int height) {
    GfxGLUTWindow::reshape(width, height);
}

void RendererApp::render() {
	switch (mRenderMode) {
		case RENDER_GL:
			mRenderer.reset(new OpenGLRenderer());
			break;
		case RENDER_REYES:
			mRenderer.reset(new ReyesRenderer());
			break;
		case RENDER_REYES_TRACE:
			mRenderer.reset(new ReyesTracingRenderer());
			break;
		case RENDER_RASTERIZE:
			mRenderer.reset(new RasterizationRenderer());
			break;
		case RENDER_RAYTRACE:
			mRenderer.reset(new RayTracingRenderer());
			break;
		case RENDER_PATH:
			mRenderer.reset(new MultipleRayTracingRenderer(nSamplesSqrt*nSamplesSqrt));
			break;
	}

	Stopwatch timer;
	timer.start();
	
	mRenderer->render(m_scene);

	std::cout << renderModeNames[mRenderMode] << " took: " << timer.cpuSeconds() << " seconds." << std::endl;
}

void RendererApp::display() {
	render();

	char buffer[1024];
	snprintf(buffer, 1024,
			 "Press \"h\" for help\n"
			 "Rendering Mode: %s\n"
			 "N Samples: %d", renderModeNames[mRenderMode], nSamplesSqrt*nSamplesSqrt);
	drawHUD(buffer);
	drawHelp(helpString);
	
	// Finish drawing scene
	glFinish();
	glutSwapBuffers();
}

void RendererApp::rotate(int dx, int dy) {
    try {
        float xfact = -g_angleFact*dy;
        float yfact = -g_angleFact*dx;
        
        Mat44f c2w = m_scene.camera->cameraToWorld();
		
        Vec3f A = c2w.A();
        Vec3f B = c2w.B();
        Vec3f C = c2w.C();
		
        // construct a coordinate system from up and viewdir
        Vec3f vRight = A;
        
        // now rotate everything
        A.rotate(float(yfact*M_PI/180.0f), B);
        A.normalize();
        B.rotate(float(xfact*M_PI/180.0f), vRight);
        C = cross(A, B).normalize();
        B = cross(C, A).normalize();
        
        c2w = Mat44f(A, B, C, c2w.D());
        
        m_scene.camera->setCameraToWorld(c2w);
		
        glutPostRedisplay();
    } catch (const std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
    }
}


void RendererApp::twist(int dx, int dy) {
    try {
        float fact = sign(dx)*g_angleFact*sqrtf((float)(dx*dx + dy*dy));
        
        Mat44f c2w = m_scene.camera->cameraToWorld();
		
        Vec3f A = c2w.A();
        Vec3f B = c2w.B();
        
        // now rotate everything
        A.rotate(float(fact*M_PI/180.0f), c2w.C());
        A.normalize();
        B = cross(c2w.C(), A).normalize();
        
        c2w = Mat44f(A, B, c2w.C(), c2w.D());
        
        m_scene.camera->setCameraToWorld(c2w);
		
        glutPostRedisplay();
    } catch (const std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
    }
}


void RendererApp::pan(int dx, int dy, int dz) {
    try {
        Mat44f c2w = m_scene.camera->cameraToWorld();
        
        c2w.setD(c2w.D() - dx*g_scaleFact*c2w.A() + dy*g_scaleFact*c2w.B() - dz*g_scaleFact*c2w.C());
		
        m_scene.camera->setCameraToWorld(c2w);
        
        glutPostRedisplay();
    } catch (const std::exception & e) {
		std::cout << "Error: " << e.what() << std::endl;
    }
}


void RendererApp::motion(int x, int y) {
	if (m_mouseButton == GLUT_LEFT_BUTTON)
		rotate(x - m_mousePressX, y - m_mousePressY);
	else if (m_mouseButton == GLUT_RIGHT_BUTTON) {
		//twist(x - m_mousePressX, y - m_mousePressY);
		pan(0, 0, 2*(x - m_mousePressX + y - m_mousePressY));
	} else if (m_mouseButton == GLUT_MIDDLE_BUTTON)
		pan(x - m_mousePressX, y - m_mousePressY, 0);
	
	m_mousePressX = x;
	m_mousePressY = y;
}


void RendererApp::keyboard(unsigned char key, int x, int y) {
	bool redisplay = true;
	switch (key) {
		case '+':
			nSamplesSqrt++;
			createScene(sceneNr);
			redisplay = false;
			break;
		case '-':
			nSamplesSqrt--;
			createScene(sceneNr);
			redisplay = false;
			break;
		case '1':
			createScene(1);
			break;
		case '2':
			createScene(2);
			break;
		case '3':
			createScene(3);
			break;
		case '4':
			createScene(4);
			break;
		case '5':
			createScene(5);
			break;
		case '6':
			createScene(6);
			break;
		case '7':
			createScene(7);
			break;
		case '8':
			createScene(8);
			break;
		case '9':
			createScene(9);
			break;
		case '0':
			createScene(0);
			break;
		case 'l':
			createScene(10);
			break;
		case 'i':
			mRenderer->saveImage(renderModeNames[mRenderMode]);
			redisplay = false;
			break;
		case 'g':
			mRenderMode = RENDER_GL;
			break;
			
		case 't':
			mRenderMode = RENDER_RAYTRACE;
			break;
			
		case 'r':
			mRenderMode = RENDER_REYES;
			break;
			
		case 'R':
			mRenderMode = RENDER_RASTERIZE;
			break;
			
		case 'x':
			mRenderMode = RENDER_REYES_TRACE;
			break;
			
		case 'p':
			mRenderMode = RENDER_PATH;
			break;
			
		case 'w':
			pan(0, 0, 100);
            break;
			
		case 's':
			pan(0, 0, -100);
            break;
			
		case 'a':
			pan(100, 0, 0);
            break;
			
		case 'd':
			pan(-100, 0, 0);
            break;
			
		case 'q':
			pan(0, 100, 0);
            break;
			
		case 'z':
			pan(0, -100, 0);
            break;		
	
			
        default:
            GfxGLUTWindow::keyboard(key, x, y);
			redisplay = false;
	}

	if (redisplay)
		glutPostRedisplay();

}