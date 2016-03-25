
#ifndef PHONG_SHADER_H
#define PHONG_SHADER_H

#include "Shader.h"

class PhongShader : public SurfaceShader
{
protected:
	Math::Color3f m_kd;
	Math::Color3f m_ks;
	float m_shininess;

public:
	PhongShader(Texture* surfaceTexture, float shininess = 100.0f);
	PhongShader(const Math::Color3f & kd = Math::Color3f(0.8f),
				const Math::Color3f & ks = Math::Color3f(0.8f),
				float shininess = 100.0f);
	virtual ~PhongShader();
	
	virtual Math::Color3f shade(const HitInfo & hit, const Scene* scene) const;
};

#endif