/*!\file PFMIO.cpp
   \author Wojciech Jarosz
 */
#if HAVE_CONFIG_H
#  include <config.h>
#endif // HAVE_CONFIG_H

#include <Img/ImageIO.h>
#include <stdexcept>
#include <string.h>

namespace Img
{

using std::string;
using namespace Math;

bool
isPFMImage(const string &name)
{
    FILE * file = fopen(name.c_str (), "rb");
    if (!file)
        return false;

    char header[3];
    size_t ret = fread(header, 1, 3, file);
    fclose(file);

    if (ret != 3)
        return false; 

    header[2] = 0;

    return (strncmp("PF", header, 2) == 0);
}


Color3f*
readPFMImage(const string &name, int * width, int * height)
{
    FILE *infile = 0;
    float *lineBuffer = 0;
    Color3f *img = 0;
    char junk;
    int ret;
    
    try
    {
        infile = fopen(name.c_str(), "rb");
        if (!infile)
            throw std::runtime_error("cannot open file.");
    
        int a = fgetc (infile);
        int b = fgetc (infile);
        fgetc(infile);
    
        if ((a != 'P') || ((b != 'F') && (b != 'f')))
            throw std::runtime_error("not a PFM image file.");
    
        b = (b == 'F');		// 'F' = RGB,  'f' = monochrome
    
        ret = fscanf(infile, "%d %d%c", width, height, &junk);
        if ((ret != 3) || (*width <= 0) || (*height <= 0))
            throw std::runtime_error("invalid width or height.");
    
        float scaleFactor;
        ret = fscanf(infile, "%f%c", &scaleFactor, &junk);
		
		if (scaleFactor > 0.0)
			throw std::runtime_error("big-endian files are not supported.");
    
        img = new Color3f [*width * *height];
    
        a = *width * (b ? 3 : 1);
        lineBuffer = new float[a];
        for (int y = 0; y < *height ; y++)
        {
            Color3f *cur = &img[y * *width];
            if (fread(lineBuffer, sizeof(float), a, infile) != (size_t) a)
                throw std::runtime_error ("cannot read pixel data.");
    
            float *temp = lineBuffer;
            for (int x = 0; x < *width; x++)
            {
                if (b)
                {   // color
                    (*cur)[0] = *temp++;
                    (*cur)[1] = *temp++;
                    (*cur)[2] = *temp++;
                }
                else
                {   // black and white
                    float c = *temp++;

                    (*cur)[0] = (*cur)[1] = (*cur)[2] = c;
                }
                cur++;
            }
        }
    
        delete [] lineBuffer;
        fclose(infile);
    
        return img;
    }
    catch (const std::exception &e)
    {
        printf("Error: Unable to read image file \"%s\": %s",
                    name.c_str (), e.what ());
        delete [] lineBuffer;
        delete [] img;
        if (infile)
            fclose (infile);
        return 0;
    }
    catch (...)
    {
        printf("Error: Unable to read image file \"%s\".", name.c_str ());
        delete [] lineBuffer;
        delete [] img;
        if (infile)
            fclose (infile);
        return 0;
    }
}


bool
writePFMImage (const ImageData &data)
{
    if (data.alpha != 0)
        printf("Warning: Alpha channels not supported in PFM images.");

    FILE *outfile = 0;
    size_t ret;
    outfile = fopen(data.filename.c_str(), "wb");
    if (!outfile)
        return false;

    fputc ('P', outfile);
    fputc ('F', outfile);
    fputc (0x0a, outfile);
    
    fprintf (outfile, "%d %d", data.xRes, data.yRes);
    fputc (0x0a, outfile);
    
	// little endian systems are assumed
    fprintf (outfile, "%f", -1.0f);
    fputc (0x0a, outfile);

    for (int y = 0; y < data.yRes; y++)
    {
        ret = fwrite(&(data.color[3*y*data.xRes]), sizeof(float), 3 * data.xRes, outfile);
    }
    
    fclose(outfile);
    
    return true;
}

} // namespace Img
