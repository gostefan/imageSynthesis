
#include "ImageTexture.h"
#include <Img/ImageIO.h>

ImageTexture::ImageTexture(const string& diffuseName, const string& specularName, const string& ambientName) {

	// Load diffuse texture
	std::cout << "Reading Diffuse Texture data...\n";
	Math::Color3f* readColors = Img::readImage(diffuseName, &sizeDX, &sizeDY);

	std::cout << "Converting Diffuse Texture data...\n";
	diffuse = vector<vector<Math::Color3f> >(sizeDY);
	int pos = 0;
	for (int i = sizeDY-1; i >= 0; i--) {
		diffuse[i] = vector<Math::Color3f>(sizeDX);
		for (int j = 0; j < sizeDX; j++) {
			diffuse[i][j] = readColors[pos];
			pos++;
		}
	}

	// Load specular texture (if any)
	if (!specularName.empty()) {
		std::cout << "Reading Specular Texture data...\n";
		readColors = Img::readImage(specularName, &sizeSX, &sizeSY);

		std::cout << "Converting Specular Texture data...\n";
		specular = vector<vector<Math::Color3f> >(sizeSY);
		pos = 0;
		for (int i = sizeSY-1; i >= 0; i--) {
			specular[i] = vector<Math::Color3f>(sizeSX);
			for (int j = 0; j < sizeSX; j++) {
				specular[i][j] = readColors[pos];
				pos++;
			}
		}
	}
	else {
		sizeSX = 0;
		sizeSY = 0;
	}

	// Load ambient texture (if any)
	if (!ambientName.empty()) {
		std::cout << "Reading Ambient Texture data...\n";
		readColors = Img::readImage(ambientName, &sizeAX, &sizeAY);

		std::cout << "Converting Ambient Texture data...\n";
		ambient = vector<vector<Math::Color3f> >(sizeAY);
		pos = 0;
		for (int i = sizeAY-1; i >= 0; i--) {
			ambient[i] = vector<Math::Color3f>(sizeAX);
			for (int j = 0; j < sizeAX; j++) {
				ambient[i][j] = readColors[pos];
				pos++;
			}
		}
	}
	else {
		sizeAX = 0;
		sizeAY = 0;
	}

	std::cout << "Texture loaded.\n";
}

Color3f ImageTexture::getAmbient(const HitInfo &hit) {
	if (sizeAX > 0 && sizeAY > 0)
		return interpolate2D(hit, ambient, sizeAX, sizeAY);
	else
		return Color3f(0);
}

Color3f ImageTexture::getDiffuse(const HitInfo &hit) {
	return interpolate2D(hit, diffuse, sizeDX, sizeDY);
}

Color3f ImageTexture::getSpecular(const HitInfo &hit) {
	if (sizeSX > 0 && sizeSY > 0)
		return interpolate2D(hit, specular, sizeSX, sizeSY);
	else
		return Color3f(0);
}

Color3f ImageTexture::interpolate1D(float pos, Color3f val0, Color3f val1, Color3f val2, Color3f val3) {
	Color3f result;

	// Interpolate each color by itself
	float d = val1.x;
	float b = (val0.x + val2.x)/2 - d;
	float a = (2*val0.x + val3.x - 3*d)/6 - b;
	float c = val2.x - a - b - d;
	result.x = ((a*pos + b)*pos + c)*pos + d;

	d = val1.y;
	b = (val0.y + val2.y)/2 - d;
	a = (2*val0.y + val3.y - 3*d)/6 - b;
	c = val2.y - a - b - d;
	result.y = ((a*pos + b)*pos + c)*pos + d;

	d = val1.z;
	b = (val0.z + val2.z)/2 - d;
	a = (2*val0.z + val3.z - 3*d)/6 - b;
	c = val2.z - a - b - d;
	result.z = ((a*pos + b)*pos + c)*pos + d;

	return result;
}

Color3f ImageTexture::interpolate2D(const HitInfo& hit, vector<vector<Color3f> >& texture, int sizeX, int sizeY) {
	float X = hit.uv.x * (sizeX-1), Y = hit.uv.y * (sizeY-1);
	int loX = static_cast<int>(floor(X)), hiX = static_cast<int>(ceil(X)), loY = static_cast<int>(floor(Y)), hiY = static_cast<int>(ceil(Y));
	int minX = max(0, loX-1), maxX = min(sizeX-1, hiX+1), minY = max(0, loY-1), maxY = min(sizeY-1, hiY+1);

	Color3f val0 = interpolate1D(X-loX, texture[minY][minX], texture[minY][loX], texture[minY][hiX], texture[minY][maxX]);
	Color3f val1 = interpolate1D(X-loX,  texture[loY][minX],  texture[loY][loX],  texture[loY][hiX],  texture[loY][maxX]);
	Color3f val2 = interpolate1D(X-loX,  texture[hiY][minX],  texture[hiY][loX],  texture[hiY][hiX],  texture[hiY][maxX]);
	Color3f val3 = interpolate1D(X-loX, texture[maxY][minX], texture[maxY][loX], texture[maxY][hiX], texture[maxY][maxX]);

	return interpolate1D(Y-loY, val0, val1, val2, val3);
}