
#include "RefractionShader.h"
#include "LambertShader.h"
#include "../Scene/LightRay.h"
#include "../Scene/EnvironmentMap.h"

RefractionShader::RefractionShader(float refractionIndex_in, const Math::Color3f& kd, float refractionPercentage_in, int maxRecursionDepth_in, SurfaceShader* recursiveShader_in) :
	recursiveShader(recursiveShader_in), refractionIndex(refractionIndex_in), refractionPercentage(refractionPercentage_in), RecursiveShader(maxRecursionDepth_in) {
	if (recursiveShader == 0) {
		myShader = true;
		recursiveShader = new LambertShader(kd);
	}
	else {
		myShader = false;
	}
}

RefractionShader::~RefractionShader() {
	if (myShader)
		delete recursiveShader;
}

Color3f RefractionShader::shade(const HitInfo & hit, const Scene* scene, stack<float>& refractionIndices) const {
	return shade(hit, scene, refractionIndices, 0);
}

Color3f RefractionShader::shade(const HitInfo & hit, const Scene* scene, stack<float>& refractionIndices, int recursionDepth) const
{
	if (recursionDepth < maxRecursionDepth) {
		std::vector<Light*> lights = scene->lights;
		Color3f color(0,0,0);
		if (refractionPercentage < 1) {
			color += (1-refractionPercentage) * recursiveShader->shade(hit, scene, refractionIndices);
		}
		
		// From in- or outside
		float dotProd = dot(hit.I, hit.N);
		bool inside = dotProd >= 0;
		float n2, n1 = refractionIndices.top();
		Vec3f normal = inside ? -hit.N : hit.N; 
		if (inside) {
			if (refractionIndices.size() > 1) // This was needed due to numeric errors when mirroring
				refractionIndices.pop();
			n2 = refractionIndices.top();
		}
		else {
			n2 = refractionIndex;
			refractionIndices.push(refractionIndex);
		}

		// Calculate refraction angle
		float theta1 = acos(dot(-hit.I, normal));
		float sinTheta2 = n1 * sin(theta1) / n2;
		float theta2 = asin(sinTheta2);

		float reflectionCoeff = reflectFresnel(n1, n2, theta1, theta2);

		// Only do the refraction if there is no total reflection
		if (sinTheta2 < 1) {
			color += (1-reflectionCoeff) * calculateRefraction(hit, scene, refractionIndices, recursionDepth, inside, normal, sinTheta2);
		}
		else {
			reflectionCoeff = 1;
		}

		// Calculate reflection part
		if (reflectionCoeff > 0.01) // This is to improve the rendering time
			color += (reflectionCoeff) * calculateReflection(hit, scene, refractionIndices, recursionDepth, inside, normal);
	
		return color;
	}
	else {
		return recursiveShader->shade(hit, scene, refractionIndices);
	}
}

Math::Color3f RefractionShader::calculateRefraction(const HitInfo & hit, const Scene* scene, stack<float> refractionIndices, int recursionDepth, bool inside, Vec3f normal, float sinTheta2) const {
	Ray r;
	r.o = hit.P;
	r.time = hit.time;

	// Calculate refraction direction
	Vec3f orthDir = (-hit.I - (dot(-hit.I, normal) * normal)).normalized();
	float c = cos(asin(sinTheta2));
	r.d = (sinTheta2 * -orthDir) + (cos(asin(sinTheta2)) * -normal);

	//loop over all scene objects and find the closest intersection
	for (unsigned int k = 0; k < scene->shapes.size(); k++) {
		scene->shapes[k]->intersect(&r);
	}

	//if ray hit something then shade it
	if (r.hit.shape != 0 && r.hit.surfaceShader != 0) {
		const RecursiveShader* rShader = dynamic_cast<const RecursiveShader*>(r.hit.surfaceShader);
		if (rShader != 0) {
			return rShader->shade(r.hit, scene, refractionIndices, recursionDepth+1);
		}
		return r.hit.surfaceShader->shade(r.hit, scene, refractionIndices);
	}
	return Color3f(scene->background->getBackground(r.d).toArray());
}
Math::Color3f RefractionShader::calculateReflection(const HitInfo & hit, const Scene* scene, stack<float> refractionIndices, int recursionDepth, bool inside, Vec3f normal) const {
	Ray r;
	r.o = hit.P;
	r.time = hit.time;

	// Calculate mirror direction
	r.d = 2*dot(hit.N, -hit.I)*hit.N + hit.I;

	//loop over all scene objects and find the closest intersection
	for (unsigned int k = 0; k < scene->shapes.size(); k++) {
		scene->shapes[k]->intersect(&r);
	}

	// Correct refraction stack
	if (inside)
		refractionIndices.push(refractionIndex);
	else
		refractionIndices.pop();

	//if ray hit something then shade it
	if (r.hit.shape != 0 && r.hit.surfaceShader != 0) {
		const RecursiveShader* rShader = dynamic_cast<const RecursiveShader*>(r.hit.surfaceShader);
		if (rShader != 0) {
			return rShader->shade(r.hit, scene, refractionIndices, recursionDepth+1);
		}
		return r.hit.surfaceShader->shade(r.hit, scene, refractionIndices);
	}
	return Color3f(scene->background->getBackground(r.d).toArray());
}

bool RefractionShader::scatterPhoton(HitInfo hit, TracePhoton& photon, Scene& scene, float continueProb) const {
	photon.origin = hit.P;
	
	// From in- or outside
	float dotProd = dot(hit.I, hit.N);
	bool inside = dotProd >= 0;
	float n2, n1 = photon.refractionIndex.top();
	Vec3f normal = inside ? -hit.N : hit.N; 
	if (inside) {
		photon.refractionIndex.pop();
		n2 = photon.refractionIndex.top();
	}
	else {
		n2 = refractionIndex;
		photon.refractionIndex.push(refractionIndex);
	}

	// Calculate refraction angle
	float theta1 = acos(dot(-hit.I, normal));
	float sinTheta2 = n1 * sin(theta1) / n2;
	float theta2 = asin(sinTheta2);

	float reflectionCoeff = sinTheta2 < 1 ? reflectFresnel(n1, n2, theta1, theta2) : 1;
	float randN = rand() / static_cast<float>(RAND_MAX);

	if (randN > reflectionCoeff) {
		Vec3f orthDir = (-hit.I - (dot(-hit.I, normal) * normal)).normalized();
		float corr = dot(orthDir, normal);
		float c = cos(asin(sinTheta2));
		photon.direction = (sinTheta2 * -orthDir) + (cos(asin(sinTheta2)) * -normal);
		if (inside) {
			photon.refractionIndex.pop();
		}
		else {
			photon.refractionIndex.push(refractionIndex);
		}
	}
	else {
		// Correct refraction stack
		if (inside)
			photon.refractionIndex.push(refractionIndex);
		else
			photon.refractionIndex.pop();
		// Calculate mirror direction
		photon.direction = 2*dot(hit.N, -hit.I)*hit.N + hit.I;
	}

	return true;
}

float RefractionShader::reflectFresnel(float n1, float n2, float thetaI, float thetaT) const {
	float nominator = n1 * cos(thetaI) - n2 * cos(thetaT);
	float denominator = n1 * cos(thetaI) + n2 * cos(thetaT);
	float sum = pow(nominator / denominator, 2);

	nominator = n2 * cos(thetaI) - n1 * cos(thetaT);
	denominator = n2 * cos(thetaI) + n1 * cos(thetaT);
	sum += pow(nominator / denominator, 2);
	return sum / 2;
}