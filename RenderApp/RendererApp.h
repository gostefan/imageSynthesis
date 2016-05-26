/*! \file RendererApp.h
    \brief Implements the Rendering Engine
    \author Wojciech Jarosz
*/

#pragma once

#include <OGL/Core.h>
#include <OGL/GLUTMaster.h>
#include <OGL/GfxGLUTWindow.h>
#include <OGL/Text.h>

#include "Math/Obj.h"
#include "Geometry/Mesh.h"
#include "Renderer/Renderer.h"
#include "Scene/PointLight.h"
#include "Scene/Scene.h"
#include "Geometry/Displacement/SineDisplacement.h"
#include "Geometry/Displacement/MultipliedSineDisplacement.h"
#include "Geometry/Displacement/ShiftedSineDisplacement.h"

#include <memory>

using namespace OGL;

class RendererApp : public GfxGLUTWindow {
	public:
		RendererApp(GLUTMaster* glutMaster,
					int width, int height,
					const char * title);
		virtual ~RendererApp();
	
		float radius;
		void createScene(int n);
		void createStillScene();
		void createMovingScene();
		void createMovingScene2();
		void createRainScene();
		void createTextureScene();
		void createBrushedScene();
		void createDOFScene();
		void createImpSampScene();
		void createNormalScene();
		void createInterestingScene();
		void render();
	
		// Event handlers
		void reshape(int width, int height);
		void display();
		void keyboard(unsigned char key, int x, int y);
		void motion(int x, int y);
	
	private:
	
		void pan(int dx, int dy, int dz);
		void rotate(int dx, int dy);
		void twist(int dx, int dy);
	
		enum {
			RENDER_GL = 0,
			RENDER_REYES,
			RENDER_RASTERIZE,
			RENDER_RAYTRACE,
			RENDER_PATH,
			RENDER_REYES_TRACE
		} m_renderMode;

		Scene m_scene;
		std::unique_ptr<Renderer> m_renderer;
		int nSamplesSqrt, sceneNr;
};
