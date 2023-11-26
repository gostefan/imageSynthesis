/*!\file PPMIO.cpp
   \author Wojciech Jarosz
 */

#include <Img/ImageIO.h>
#include <Math/Core.h>
#include <stdexcept>
#include <string.h>

namespace Img
{

using std::string;
using namespace Math;

namespace
{

struct RGB
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

}

bool
isPPMImage (const string &name)
{
    FILE * file;
    if (fopen_s(&file, name.c_str (), "rb") != 0 || !file)
        return false;

    char header[3];
    size_t ret = fread (header, 1, 3, file);
    fclose (file);

    if (ret != 3)
        return false;
    header[2] = 0;

    return (strncmp("P6", header, 2) == 0);
}


Color3f*
readPPMImage (const string &name, int * width, int * height)
{
    FILE *infile = 0;
    Color3f *img = 0;
    int colors;
    float invColors;
    char buffer[256];
    RGB *buf = 0;

    try
    {
        if (fopen_s(&infile, name.c_str(), "rb") != 0 || !infile)
            throw std::runtime_error ("cannot open file.");
    
        if ((fgets(buffer, 256, infile) == 0) || (buffer[0] != 'P') || (buffer[1] != '6'))
            throw std::runtime_error("image is not a binary PPM file.");
    
        // skip comments
        do {fgets(buffer, sizeof (buffer), infile);} while (buffer[0] == '#');
    
        // read image size
        sscanf_s(buffer, "%d %d", width, height);
        
        // skip comments
        do {fgets (buffer, sizeof (buffer), infile);} while (buffer[0] == '#');
    
        // read maximum pixel value (usually 255)
        sscanf_s(buffer, "%d", &colors);
        invColors = 1.0f/colors;
    
        if (colors != 255)
            throw std::runtime_error ("maximum pixel value must be 255.");
    
        img = new Color3f [*width * *height];
    
        buf = new RGB[*width];
        for (int y = 0; y < *height; ++y)
        {
            if (fread(buf, *width * sizeof(RGB), 1, infile) != 1)
                throw std::runtime_error ("cannot read pixel data.");
    
            RGB *cur = buf;
            Color3f *curLine = &img[y * *width];
            for (int x = 0; x < *width; x++)
            {
                curLine[x][0] = cur->r * invColors;
                curLine[x][1] = cur->g * invColors;
                curLine[x][2] = cur->b * invColors;
                cur++;
            }
        }
        delete [] buf;
    
        fclose(infile);
        return img;
    }
    catch (const std::exception &e)
    {
        printf("Error: Unable to read image file \"%s\": %s",
                    name.c_str (), e.what ());
        delete [] buf;
        delete [] img;
        if (infile)
            fclose (infile);
        return 0;
    }
    catch (...)
    {
        printf("Error: Unable to write image file \"%s\"", name.c_str ());
        delete [] buf;
        delete [] img;
        if (infile)
            fclose (infile);
        return 0;
    }
}


bool
writePPMImage (const ImageData &data)
{
    if (data.alpha != 0)
        printf("Warning: Alpha channels not supported in PPM images.");

    FILE *outfile = 0;

    try
    {
        if (fopen_s(&outfile, data.filename.c_str(), "wb") != 0 && !outfile)
            throw std::runtime_error ("cannot open file.");
    
        // write header
        fprintf (outfile, "P6\n");
        fprintf (outfile, "%d %d\n", data.xRes, data.yRes);
        fprintf (outfile, "255\n" );
    
        for (int y = 0; y < data.yRes; ++y)
        {
            float *buf = &data.color[3*y*data.xRes];
            for (int x = 0; x < data.xRes; x++)
            {
                unsigned char r = data.gTable.lookup (*(buf++)),
                              g = data.gTable.lookup (*(buf++)),
                              b = data.gTable.lookup (*(buf++));
                if ((fwrite(&r, 1, 1, outfile) != 1) ||
                    (fwrite(&g, 1, 1, outfile) != 1) ||
                    (fwrite(&b, 1, 1, outfile) != 1))
                    throw std::runtime_error ("cannot write pixel data.");
            }
        }
    
        fclose (outfile);
        return true;
    }
    catch (const std::exception &e)
    {
        printf("Error: Unable to write image file \"%s\": %s",
                    data.filename.c_str (), e.what ());
        if (outfile)
            fclose (outfile);
        return false;
    }
    catch (...)
    {
        printf("Error: Unable to write image file \"%s\"",
                    data.filename.c_str ());
        if (outfile)
            fclose (outfile);
        return false;
    }
}

} // namespace Img
