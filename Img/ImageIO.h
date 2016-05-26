/*! \file ImageIO.h
    \author Wojciech Jarosz
*/

#pragma once

#include <Math/Color.h>
#include <Img/GammaTable.h>
#include <Util/Fwd.h>
#include <Math/Fwd.h>
#include <string>
#include <map>

namespace Img {
                 
	class ImageData {
		public:
			ImageData(const std::string& filename, 
				Util::Array2D<Math::Color4f> & rgbaBuffer);

			ImageData(const std::string& filename,
					   float *color,
					   int xRes, int yRes,
					   float *alpha,
					   int xOffset, int yOffset,
					   int totalXRes, int totalYRes,
					   float gamma = 1.0f, float imageScale = 1.0f) :
				filename(filename),
				color(color), alpha(alpha), xRes(xRes), yRes(yRes),
				xOffset(xOffset), yOffset(yOffset),
				totalXRes(totalXRes), totalYRes(totalYRes),
				gTable(gamma, imageScale) {}

			ImageData(const std::string& filename,
					   float *color,
					   int xRes, int yRes,
					   float *alpha,
					   int xOffset, int yOffset,
					   int totalXRes, int totalYRes,
					   const GammaTable &g) :
				filename(filename),
				color(color), alpha(alpha), xRes(xRes), yRes(yRes),
				xOffset(xOffset), yOffset(yOffset),
				totalXRes(totalXRes), totalYRes(totalYRes),
				gTable(g) {}
    
			ImageData(const std::string& filename,
					   float *color,
					   int xRes, int yRes,
					   float gamma = 1.0f, float imageScale = 1.0f,
					   float *alpha = 0,
					   int xOffset = 0, int yOffset = 0) :
				filename(filename),
				color(color), alpha(alpha), xRes(xRes), yRes(yRes),
				xOffset(xOffset), yOffset(yOffset),
				totalXRes(xRes), totalYRes(yRes),
				gTable(gamma, imageScale) {}
    
			ImageData(const std::string& filename,
					   float *color,
					   int xRes, int yRes,
					   const GammaTable &g,
					   float *alpha = 0,
					   int xOffset = 0, int yOffset = 0) :
				filename(filename),
				color(color), alpha(alpha), xRes(xRes), yRes(yRes),
				xOffset(xOffset), yOffset(yOffset),
				totalXRes(xRes), totalYRes(yRes),
				gTable(g) {}

			std::string filename;
			float *color;
			float *alpha;
			int xRes, yRes;
			int xOffset, yOffset;
			int totalXRes, totalYRes;
			GammaTable gTable;
	};

	Math::Color3f* readImage(const std::string &name, int *width, int *height);
	bool writeImage(const ImageData &data);

	bool writePFMImage(const ImageData &data);
	bool writePPMImage(const ImageData &data);
	bool writeHDRImage(const ImageData &data);

} // namespace Img