/*!\file ImageIO.cpp
   \author Wojciech Jarosz
 */

#include <Img/ImageIO.h>
#include <stdexcept>
#include <Util/Array2D.h>


namespace {
	inline bool strEq(const char* exp, const char* test) {
	#ifdef _WIN32
		return !_stricmp(exp, test);
	#else
		return !strncasecmp(exp, test, strlen(exp));
	#endif
	}
} // namespace

	namespace Img {

	using std::string;
	using Util::Array2D;
	using Math::Color4f;

	ImageData::ImageData(const string & filename, Array2D<Color4f> & rgbaBuffer) :
			filename(filename) {
		totalXRes = xRes = rgbaBuffer.getSizeX();
		totalYRes = yRes = rgbaBuffer.getSizeY();

		xOffset = yOffset = 0;
		gTable = GammaTable(1,1);

		color = new float[xRes*yRes*3];
		alpha = NULL;

		for (int x = 0; x < xRes; x++) {
			for (int y = 0; y < yRes; y++) {
				for (int i = 0; i < 3; i++) 
					color[3*(y*xRes+x)+i] = rgbaBuffer(x,yRes-y-1)[i];
			}
		}
	}


	extern bool isPFMImage(const string &name);
	extern Math::Color3f* readPFMImage(const string &name, int *width, int *height);

	extern bool isPPMImage(const string &name);
	extern Math::Color3f* readPPMImage(const string &name, int *width, int *height);

	extern bool isHDRImage(const string &name);
	extern Math::Color3f* readHDRImage(const string &name, int *width, int *height);


	Math::Color3f* readImage(const string &name, int *width, int *height) {
		// first see if the file exists
		FILE* file;
		if (fopen_s(&file, name.c_str(), "rb") != 0 || !file) {
			printf("Error: Unable to read image file \"%s\": "
					   "file does not exist.", name.c_str());
			return 0;
		}
		fclose(file);

		if (isHDRImage(name))
			return readHDRImage(name, width, height);
		else if (isPFMImage(name))
			return readPFMImage(name, width, height);
		else if (isPPMImage(name))
			return readPPMImage(name, width, height);
		else {
			printf("Error: Unable to read image file \"%s\": "
						"not a recognized file format.", name.c_str());
			return nullptr;
		}
	}


	bool writeImage(const ImageData &data) {
		bool ok = false;
		std::cout << "Exporting '" << data.filename << "'... ";
		try {
			string::size_type idx = data.filename.rfind('.');
			if (idx == string::npos)
				throw std::runtime_error("Unable to deduce format, "
										  "no file extension given.");

			string ext = data.filename.substr(idx+1);
			const char * extC = ext.c_str();

			if (strEq(extC, "pfm"))
				ok = writePFMImage(data);
			else if (strEq(extC, "ppm"))
				ok = writePPMImage(data);
			else if (strEq(extC, "rgbe") || strEq(extC, "pic") || strEq(extC, "hdr"))
				ok = writeHDRImage(data);
			else
				throw std::runtime_error("Unrecognized/unsupported extension.");
		} catch (const std::exception &e) {
			printf("Warning: \"%s\": %s Writing PFM file instead.",
						  data.filename.c_str(), e.what());
			return writePFMImage(data);
		}

		std::cout << (ok ? "done." : "failed." ) << std::endl;
    
		return ok;
	}

} // namespace Img
