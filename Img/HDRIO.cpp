/*!\file HDRIO.cpp
   \author Wojciech Jarosz
 */
#if HAVE_CONFIG_H
#  include <config.h>
#endif // HAVE_CONFIG_H

#include <Img/ImageIO.h>
#include <Math/RGBE.h>
#include <stdexcept>
#include <string.h>

namespace Img
{

using std::string;
using namespace Math;

namespace
{

// minimal header reading.  modify if you want to parse more information
bool
HDRReadHeader (FILE* fp, int* width, int* height)
{
    char buf[128];
    size_t size = sizeof(buf)/sizeof(buf[0]);

    if (!fgets(buf, size, fp))
        return false;

    if ((buf[0] != '#')||(buf[1] != '?'))
    {
        //if you want to require the magic token then uncomment the next line
        //return rgbe_error(rgbe_format_error,"bad initial token");
    }

    while (true)
    {
        if (strcmp (buf,"\n") == 0)
            break;       // format found so break out of loop

        if (fgets (buf, size, fp) == 0)
            return false;
    }

    if (fgets (buf, size, fp) == 0)
        return false;

	if (sscanf (buf, "-Y %d +X %d", height, width) < 2)
        return false;
    return true;
}


// simple read routine.  will not correctly handle run length encoding
bool
HDRReadPixels (FILE *fp, Color3f * img, int rasterPos, int numPixels)
{
    unsigned char rgbe[4];

    while (numPixels--)
    {
        if (fread (rgbe, sizeof (rgbe), 1, fp) < 1)
            return false;

        RGBEToColor3 (&(img[rasterPos]), rgbe);
        rasterPos++;
    }
    return true;
}


bool
HDRReadPixelsRLE (FILE *fp, Color3f * img, int imageWidth, int imageHeight)
{
    unsigned char rgbe[4], *scanlineBuffer = 0, *ptr, *ptrEnd;
    Color3f c;
    int i, count;
    unsigned char buf[2];
    int rasterPos = 0;

    if (imageWidth < 8 || imageWidth > 0x7fff)
        // run length encoding is not allowed so read flat
        return HDRReadPixels (fp, img, 0, imageWidth * imageHeight);

    scanlineBuffer = 0;

    // read in each successive scanline
    while (imageHeight > 0)
    {
        if (fread (rgbe, sizeof(rgbe), 1, fp) < 1)
        {
            delete [] scanlineBuffer;
            return false;
        }

        if (rgbe[0] != 2 || rgbe[1] != 2 || rgbe[2] & 0x80)
        {
            // this file is not run length encoded
            RGBEToColor3 (&(img[rasterPos]), rgbe);

            rasterPos++;
            delete [] scanlineBuffer;
            return HDRReadPixels (fp, img, rasterPos,
                                   imageWidth * imageHeight - 1);
        }

        if ((((int)rgbe[2]) << 8 | rgbe[3]) != imageWidth)
        {
            delete [] scanlineBuffer;
            return false;
        }

        if (scanlineBuffer == 0)
            scanlineBuffer = new unsigned char [4*imageWidth];
            
        if (scanlineBuffer == 0) 
            return false;

        ptr = &scanlineBuffer[0];

        // read each of the four channels for the scanline into the buffer
        for (i = 0; i < 4; i++)
        {
            ptrEnd = &scanlineBuffer[(i+1)*imageWidth];
            while (ptr < ptrEnd)
            {
                if (fread (buf, sizeof(buf[0])*2, 1, fp) < 1)
                {
                    delete [] scanlineBuffer;
                    return false;
                }
                if (buf[0] > 128)
                {
                    // a run of the same value
                    count = buf[0]-128;

                    if (count == 0 || count > ptrEnd - ptr)
                    {
                        delete [] scanlineBuffer;
                        return false;
                    }

                    while (count-- > 0)
                        *ptr++ = buf[1];
                }
                else
                {
                    // a non-run
                    count = buf[0];

                    if (count == 0 || count > ptrEnd - ptr)
                    {
                        delete [] scanlineBuffer;
                        return false;
                    }

                    *ptr++ = buf[1];

                    if (--count > 0)
                    {
                        if (fread (ptr, sizeof(*ptr)*count, 1, fp) < 1)
                        {
                            delete [] scanlineBuffer;
                            return false;
                        }

                        ptr += count;
                    }
                }
            }
        }

        // now convert data from buffer into floats
        for (i = 0; i < imageWidth; i++)
        {
            rgbe[0] = scanlineBuffer[i];
            rgbe[1] = scanlineBuffer[i+imageWidth];
            rgbe[2] = scanlineBuffer[i+2*imageWidth];
            rgbe[3] = scanlineBuffer[i+3*imageWidth];
            
            RGBEToColor3 (&(img[rasterPos]), rgbe);
            rasterPos++;
        }
        imageHeight--;
    }

    delete [] scanlineBuffer;
    return true;
}


// default minimal header. modify if you want more information in header
bool
HDRWriteHeader (FILE *fp, int width, int height)
{
    if (fprintf (fp,"#?%s\n", "RGBE") < 0)
        return false;
    if (fprintf (fp,"FORMAT=32-bit_rle_rgbe\n\n") < 0)
        return false;
    if (fprintf (fp, "-Y %d +X %d\n", height, width) < 0)
        return false;

    return true;
}


// simple write routine that does not use run length encoding
// These routines can be made faster by allocating a larger buffer and
// fread-ing and fwrite-ing the data in larger chunks 
bool
HDRWritePixels (FILE *fp, float *data, int numPixels)
{
    unsigned char rgbe[4];

    while (numPixels-- > 0)
    {
        floatToRGBE (rgbe, data[0], data[1], data[2]);
        if (fwrite(rgbe, sizeof(rgbe), 1, fp) < 1)
            return false;
            
        data += 3;
    }
    return true;
}


// The code below is only needed for the run-length encoded files.
// Run length encoding adds considerable complexity but does
// save some space.  For each scanline, each channel (r,g,b,e) is
// encoded separately for better compression. 
bool
HDRWriteBytesRLE (FILE *fp, unsigned char *data, int numbytes)
{
#define MINRUNLENGTH 4
    int cur, begRun, runCount, oldRunCount, noRunCount;
    unsigned char buf[2];

    cur = 0;
    while (cur < numbytes)
    {
        begRun = cur;
        
        // find next run of length at least 4 if one exists
        runCount = oldRunCount = 0;
        while ((runCount < MINRUNLENGTH) && (begRun < numbytes))
        {
            begRun += runCount;
            oldRunCount = runCount;
            runCount = 1;
            while ((begRun + runCount < numbytes) && (runCount < 127)
                    && (data[begRun] == data[begRun + runCount]))
                runCount++;
        }
        
        // if data before next big run is a short run then write it as such
        if ((oldRunCount > 1) && (oldRunCount == begRun - cur))
        {
            buf[0] = 128 + oldRunCount;   /*write short run*/
            buf[1] = data[cur];
            if (fwrite(buf,sizeof(buf[0])*2,1,fp) < 1)
                return false;
            cur = begRun;
        }
        
        // write out bytes until we reach the start of the next run
        while (cur < begRun)
        {
            noRunCount = begRun - cur;
            if (noRunCount > 128)
                noRunCount = 128;
                
            buf[0] = noRunCount;
            if (fwrite(buf,sizeof(buf[0]),1,fp) < 1)
                return false;
            
            if (fwrite(&data[cur],sizeof(data[0])*noRunCount,1,fp) < 1)
                return false;
            cur += noRunCount;
        }
        
        // write out next run if one was found
        if (runCount >= MINRUNLENGTH)
        {
            buf[0] = 128 + runCount;
            buf[1] = data[begRun];
            if (fwrite(buf,sizeof(buf[0])*2,1,fp) < 1)
                return false;
            cur += runCount;
        }
    }
    return true;
#undef MINRUNLENGTH
}


bool
HDRWritePixelsRLE (FILE *fp, float *data, int xRes, int yRes)
{
    unsigned char rgbe[4];
    unsigned char *buffer;
    int i;

    if ((xRes < 8) || (xRes > 0x7fff))
        // run length encoding is not allowed so write flat
        return HDRWritePixels (fp, data, xRes*yRes);

    buffer = new unsigned char [4*xRes];
    if (buffer == 0)
        // no buffer space so write flat
        return HDRWritePixels(fp,data,xRes*yRes);

    while (yRes-- > 0)
    {
        rgbe[0] = 2;
        rgbe[1] = 2;
        rgbe[2] = xRes >> 8;
        rgbe[3] = xRes & 0xFF;
        
        if (fwrite(rgbe, sizeof(rgbe), 1, fp) < 1)
        {
            delete [] buffer;
            return false;
        }
        
        for (i = 0; i < xRes; ++i)
        {
            floatToRGBE (rgbe, data[0], data[1], data[2]);
            buffer[i] = rgbe[0];
            buffer[i+xRes] = rgbe[1];
            buffer[i+2*xRes] = rgbe[2];
            buffer[i+3*xRes] = rgbe[3];
            data += 3;
        }
        
        // write out each of the four channels separately run length encoded
        // first red, then green, then blue, then exponent
        for (i = 0; i < 4; ++i)
        {
            if (!HDRWriteBytesRLE(fp,&buffer[i*xRes], xRes))
            {
                delete [] buffer;
                return false;
            }
        }
    }
    delete [] buffer;
    return true;
}

} // namespace
	

bool
isHDRImage (const string &name)
{
    FILE * file = fopen (name.c_str (), "rb");
    if (!file)
        return false;
        
    int width, height;
    bool ok = HDRReadHeader (file, &width, &height);
    fclose (file);
    return ok;
}


Color3f * 
readHDRImage (const string &name, int * width, int * height)
{
    FILE * file = 0;
    Color3f* img = 0;
     
    try
    {
        file = fopen (name.c_str (), "rb");
        if (!file)
            throw std::runtime_error ("cannot open file.");
    
        if (!HDRReadHeader (file, width, height))
            throw std::runtime_error ("cannot read header.");
    
        img = new Color3f [*width * *height];
        if (!HDRReadPixelsRLE (file, img, *width, *height))
            throw std::runtime_error ("cannot read pixel data.");
    
        return img;
    }
    catch (const std::exception &e)
    {
        printf("Error: Unable to read image file \"%s\": %s",
                    name.c_str (), e.what ());
        if (file)
        {
            fclose (file);
            delete [] img;
        }
        return 0;
    }
    catch (...)
    {
        printf("Error: Unable to read image file \"%s\"", name.c_str ());
        if (file)
        {
            fclose (file);
            delete [] img;
        }
        return 0;
    }
}


bool
writeHDRImage (const ImageData &data)
{
    if (data.alpha != 0)
        printf("Warning: Alpha channels not supported in Radiance images.");

    FILE* outfile = 0;
     
    try
    {
        outfile = fopen (data.filename.c_str(), "wb");
        if (!outfile)
            throw std::runtime_error ("cannot open file.");
            
        if (!HDRWriteHeader (outfile, data.xRes, data.yRes))
            throw std::runtime_error ("cannot write header.");
        if (!HDRWritePixelsRLE (outfile, data.color, data.xRes, data.yRes))
            throw std::runtime_error ("cannot write pixel data.");
        
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
