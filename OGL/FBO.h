/*!
	\file FBO.h
	\author Wojciech Jarosz
	\brief A class that encapsulates OpenGL framebuffer objects.
	\details This code is based on the FrameBuffer class written by Chris Wyman.
	The original descriptions follows.

	framebufferObject.h

	-------------------
	 
	This is the frame-work for general purpose
	initialization of a framebuffer object,
	as specified in the OpenGL extension:
	GL_EXT_FRAMEBUFFER_OBJECT
	Since this is an OpenGL extension, not WGL,
	it should be much more portable (and
	supposedly) faster than p-buffers and
	render-to-texture.
	 
	Chris Wyman (4/27/2005)
 */

#pragma once

#include <OGL/Core.h>

#define FBO_DEPTH         0
#define FBO_STENCIL       10
#define FBO_COLOR(x)      (20+(x))
#define FBO_COLOR0        20
#define FBO_COLOR1        21
#define FBO_COLOR2        22
#define FBO_COLOR3        23
#define FBO_COLOR4        24

/*!
	A class that encapsulates and OpenGL framebuffer object. 
 */
class FrameBuffer
{
private:
	GLuint m_ID;         //!< from glGenFramebuffersEXT()
	GLuint *m_colorIDs;  //!< these are the GL texture IDs for the color buffers
	GLuint m_depthID;    //!< the GL texture ID for the m_depth buffer (_OR_ the m_depth buffer ID from glBindRenderbufferEXT)
	GLuint m_stencilID;  //!< the GL buffer ID for the stencil buffer (from glBindRenderbufferEXT)
	GLint m_maxColorBuffers;   //!< the max number of color buffer textures (i.e., size of m_colorIDs[] array)
	GLuint m_prevFrameBuf;     //!< a very primitive method for allowing nested bindBuffer() calls. (BE CAREFUL)
	int m_width, m_height;       //!< m_width & m_height of ALL textures/buffers in framebuffer object.  (MAY BE INCORRECT if used with the default Framebuffer() constructor!)
	char m_FBOName[80];         //!< a simple ascii text name for the buffer -- useful for debugging

	GLenum *m_colorType;
	GLenum m_depthType, m_stencilType;
	unsigned int m_includedBuffers;
	int m_automaticMipmapsEnabled;
	int m_numColorAttachments;

	// Test values for use with texture array FBOs.  These may be broken, and should not be exposed!
	int m_depth;

public:
	FrameBuffer(const char *name=0);                           // avoid the use of this constructor whenever possible
	FrameBuffer(int m_width, int m_height, const char *name=0);    // use this constructor if you want to setup the textures yourself
	FrameBuffer(GLenum type, int w, int h, int d, 
				GLuint colorBufType, int numColorBufs, int hasZbuf, 
		        bool enableAutomaticMipmaps, const char *name=0);
	FrameBuffer(int test);
	~FrameBuffer();

	// This is useful for debugging.
	//    It displays (in the currently active framebuffer [WARNING: CANNOT BE THIS ONE])
	//    the specified attachment as a full screen image (i.e., using gluOrtho2D()).
	//    This overwrites whatever was currently there.
	void displayAsFullScreenTexture(int attachment, float lod=0.0f);  // attachment is one of the FBO_xxx #define's frome above
	void displayAlphaAsFullScreenTexture(int attachment, float lod=0.0f);
	void blitFramebuffer(int attachment);

	//!	Clears all the buffers in the framebuffer.
	/*! 
		Clears all the buffers in the framebuffer (based on the values currently set
		by fuctions like glClearColor(), etc).  This function simply calls glClear()
		with the appropriate bits for the buffers allocated in this FBO!
	*/
	inline void clearBuffers() {glClear(m_includedBuffers);}

	//! Check to see if there are any errors.
	/*! 
		Returns GL_FRAMEBUFFER_COMPLETE_EXT if OK.
		Prints messages to stdout if printMessage == 1
	 */
    GLenum checkFramebufferStatus(int printMessage=0);

	// Attach textures to various attachment points
	int attachColorTexture(GLuint colorTexID, int colorBuffer=0);
	int attachDepthTexture(GLuint depthTexID);
	int attachStencilTexture(GLuint stencilTexID);  // CURRENTLY UNSUPPORTED. DO NOT USE!

	// Attach renderbuffers to various attachment points
	//    (note these SHOULD replace textures, but it may not be guaranteed,
	//     so you might want to unbind textures before binding a renderbuffer)
	int attachColorBuffer(GLuint colorBufID, int colorBuffer=0);   // Use only if you know what you're doing!
	int attachDepthBuffer(GLuint depthBufID);                      // Use only if you know what you're doing!
	int attachStencilBuffer(GLuint stencilBufID);                  // Use only if you know what you're doing!

	// Functionality for drawing custom mipmap levels.
	void drawToColorMipmapLevel(GLuint colorBuffer, GLuint level); // Use only if you know what you're doing!
	void doneDrawingMipmapLevels();                            // Use only if you know what you're doing!

	// attachment is one of the FBO_xxx #defined from above
	//   NOTE: This only works if automatic mipmapping has been enabled for this attachment! 
	//         (and OGL supports it -- though we try for all attachments colorBuf, depthBuf, stencilBuf)
	void automaticallyGenerateMipmaps(int attachment);            

	// Bind/unbind the current framebuffer.  These functions store the currently
	//    bound framebuffer during a bindBuffer() and rebind it upon an unbindBuffer()
	//    (which allows for a very basic nesting of bindBuffer() calls)
	GLuint bindBuffer();   // Commands after this draw into THIS framebuffer.
	int unbindBuffer();    // Commands after this draw into the normal framebuffer (e.g., screen)

	// Queries to return the texture/renderbuffer ID of the various attachments
	//
	//    You may thus use one of the color buffers as a GL texture using:
	//          glBindTexture( GL_TEXTURE_2D, framebuffer->colorTextureID( 0 ) );
	//    Beware you may NOT have this framebuffer bound (via bindBuffer()) at the 
	//    same time you're using one of the textures (via glBindTexture()).  This means
	//    when you're done using framebuffer->colorTextureID(0), you should call
	//          glBindTexture( GL_TEXTURE_2D, 0 );
	inline GLuint colorTextureID(int level=0) { return (level < m_maxColorBuffers && level >= 0 ? m_colorIDs[level] : -1); }
	inline GLuint depthTextureID()        { return m_depthID; }
	inline GLuint stencilTextureID()      { return m_stencilID; }
	GLuint textureID(int attachment);         // attachment is one of the FBO_xxx #define's frome above
	GLenum textureType(int attachment);

	inline int numColorTextures()         { return m_numColorAttachments; }

	// Gets some identifying information about the framebuffer object.
	inline int width()	const				{ return m_width; }      
	inline int height() const				{ return m_height; }   
	inline float aspectRatio() const		{ return m_width/(float)m_height; }
	inline GLuint bufferID() const			{ return m_ID; }
	inline const char *name()				{ return m_FBOName; }

	//! Allows you to set the size of the FBO.
	/*! Particularly useful if you use the default constructor, 
		and want to set it up correctly afterwards!
	 */
	inline void setSize(int newWidth, int newHeight) {m_width = newWidth; m_height = newHeight;}

	//! Resize an FBO that already exists.
	void resizeExistingFBO(int newWidth, int newHeight);
};