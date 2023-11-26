/************************************************
 ** framebufferObject.cpp                       **
 ** ---------------------                       **
 **                                             **
 ** This is the frame-work for general purpose  **
 **   initialization of a framebuffer object,   **
 **   as specified in the OpenGL extension:     **
 **       GL_EXT_FRAMEBUFFER_OBJECT             **
 **                                             **
 ** Since this is an OpenGL extension, not WGL, **
 **   it should be much more portable (and      **
 **   supposedly) faster than p-buffers and     **
 **   render-to-texture.                        **
 **                                             **
 ** Chris Wyman (4/27/2005)                     **
 ************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <OGL/FBO.h>

FrameBuffer::FrameBuffer(const char *name) : m_depth(-1), m_automaticMipmapsEnabled(0)
{
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &m_maxColorBuffers);
	m_colorIDs = new GLuint[m_maxColorBuffers];
	m_colorType = new GLenum[m_maxColorBuffers];
	m_numColorAttachments = 0;
	m_depthID = 0;
	m_stencilID = 0;
	for (int i=0; i<m_maxColorBuffers; i++)
	{
		m_colorIDs[i] = 0;
		m_colorType[i] = GL_TEXTURE_2D;
	}
	m_prevFrameBuf = 0;
	m_width = m_height = 0;
	m_includedBuffers = 0;
	m_depthType = m_stencilType = GL_TEXTURE_2D;
	glGenFramebuffersEXT(1, &m_ID);
	
	if (!name)
		sprintf_s(m_FBOName, 80, "Framebuffer %d", m_ID);
	else
		strncpy_s(m_FBOName, 80, name, 79);
}

FrameBuffer::FrameBuffer(int w, int h, const char *name) :
	m_width(w), m_height(h), m_depth(-1), m_automaticMipmapsEnabled(0)
{
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &m_maxColorBuffers);
	m_colorIDs = new GLuint[m_maxColorBuffers];
	m_colorType = new GLenum[m_maxColorBuffers];
	m_depthID = m_stencilID = 0;
	m_numColorAttachments = 0;
	for (int i=0; i<m_maxColorBuffers; i++)
	{
		m_colorIDs[i] = 0;
		m_colorType[i] = GL_TEXTURE_2D;
	}
	m_prevFrameBuf = 0;
	m_includedBuffers = 0;
	m_depthType = m_stencilType = GL_TEXTURE_2D;
	glGenFramebuffersEXT(1, &m_ID);
	
	if (!name)
		sprintf_s(m_FBOName, 80, "Framebuffer %d", m_ID);
	else
		strncpy_s(m_FBOName, 80, name, 79);
}

FrameBuffer::FrameBuffer(GLenum type, int w, int h, int d, 
						 GLuint colorBufType, int numColorBufs, int hasZbuf, 
						 bool enableAutomaticMipmaps, const char *name) :
	m_width(w), m_height(h), m_depth(d), 
	m_automaticMipmapsEnabled(enableAutomaticMipmaps?1:0)
{
	if (type == GL_TEXTURE_1D || type == GL_TEXTURE_3D)
		printf("Warning!  FrameBuffer constructor called with untested texture type!\n");
	
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &m_maxColorBuffers);
	m_colorIDs = new GLuint[m_maxColorBuffers];
	m_colorType = new GLenum[m_maxColorBuffers];
	m_depthID = m_stencilID = m_prevFrameBuf = m_includedBuffers = 0;
	for (int i=0; i<m_maxColorBuffers; i++)
	{
		m_colorIDs[i] = 0;
		m_colorType[i] = type;
	}
	m_depthType = m_stencilType = type;
	
	if (!name) sprintf_s(m_FBOName, 80, "Framebuffer %d", m_ID);
	else strncpy_s(m_FBOName, 80, name, 79);
	
	glGenFramebuffersEXT(1, &m_ID);
	

	m_numColorAttachments = numColorBufs;
	if (numColorBufs > 0)
	{
		m_includedBuffers |= GL_COLOR_BUFFER_BIT;
		glGenTextures(numColorBufs, m_colorIDs);
		
		for (int i=0; i<numColorBufs; i++)
		{
			glBindTexture(type, m_colorIDs[i]);
			glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(type, GL_GENERATE_MIPMAP, m_automaticMipmapsEnabled > 0 ? GL_TRUE : GL_FALSE);
			if (type == GL_TEXTURE_CUBE_MAP)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, colorBufType, m_width, m_height, 0, GL_RGBA, GL_FLOAT, NULL);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, colorBufType, m_width, m_height, 0, GL_RGBA, GL_FLOAT, NULL);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, colorBufType, m_width, m_height, 0, GL_RGBA, GL_FLOAT, NULL);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, colorBufType, m_width, m_height, 0, GL_RGBA, GL_FLOAT, NULL);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, colorBufType, m_width, m_height, 0, GL_RGBA, GL_FLOAT, NULL);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, colorBufType, m_width, m_height, 0, GL_RGBA, GL_FLOAT, NULL);
			}
			else if (type == GL_TEXTURE_2D_ARRAY_EXT || type == GL_TEXTURE_3D)
				glTexImage3D(type, 0, colorBufType, m_width, m_height, m_depth, 0, GL_RGBA, GL_FLOAT, NULL);
			else if (type == GL_TEXTURE_2D || type == GL_TEXTURE_1D_ARRAY_EXT)
				glTexImage2D(type, 0, colorBufType, m_width, m_height, 0, GL_RGBA, GL_FLOAT, NULL);
			else if (type == GL_TEXTURE_1D)
				glTexImage1D(type, 0, colorBufType, m_width, 0, GL_RGBA, GL_FLOAT, NULL);
			if (enableAutomaticMipmaps) glGenerateMipmapEXT(type);
			bindBuffer();
			
			glFramebufferTextureEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT+i, m_colorIDs[i], 0);
			unbindBuffer();
		}
	}
	if (hasZbuf > 0)
	{
		m_includedBuffers |= GL_DEPTH_BUFFER_BIT;
		glGenTextures(1, &m_depthID);
		
		glBindTexture(type, m_depthID);
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(type, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
		if (type == GL_TEXTURE_CUBE_MAP)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		}
		else if (type == GL_TEXTURE_2D_ARRAY_EXT || type == GL_TEXTURE_3D)
			glTexImage3D(type, 0, GL_DEPTH_COMPONENT, m_width, m_height, m_depth, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		else if (type == GL_TEXTURE_2D || type == GL_TEXTURE_1D_ARRAY_EXT)
			glTexImage2D(type, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		else if (type == GL_TEXTURE_1D)
			glTexImage1D(type, 0, GL_DEPTH_COMPONENT, m_width, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		bindBuffer();
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, m_depthID, 0);
		unbindBuffer();
	}
	
	glBindTexture(type, 0);
}


FrameBuffer::FrameBuffer(int test) :
	m_width(512), m_height(512), m_depth(-1), m_automaticMipmapsEnabled(0)
{
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &m_maxColorBuffers);
	m_colorIDs = new GLuint[m_maxColorBuffers]; m_colorType = new GLenum[m_maxColorBuffers];
	m_depthID = m_stencilID = m_prevFrameBuf = m_includedBuffers = 0;
	for (int i=0; i<m_maxColorBuffers; i++)
	{
		m_colorIDs[i] = 0;
		m_colorType[i] = GL_TEXTURE_2D_ARRAY_EXT;
	}
	m_depthType = m_stencilType = GL_TEXTURE_2D_ARRAY_EXT;
	sprintf_s(m_FBOName, 80, "Test CubeMap Framebuffer");
	
	m_numColorAttachments = 1;
	m_includedBuffers = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
	glGenFramebuffersEXT(1, &m_ID);
	
	glGenTextures(1, m_colorIDs);
	glBindTexture(GL_TEXTURE_2D_ARRAY_EXT, m_colorIDs[0]);
	glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage3D(GL_TEXTURE_2D_ARRAY_EXT, 0, GL_LUMINANCE, m_width, m_height, 6, 0, GL_RGBA, GL_FLOAT, NULL);
	
	glGenTextures(1, &m_depthID);
	glBindTexture(GL_TEXTURE_2D_ARRAY_EXT, m_depthID);
	glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY_EXT, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);	
	glTexImage3D(GL_TEXTURE_2D_ARRAY_EXT, 0, GL_DEPTH_COMPONENT, m_width, m_height, 6, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	
	glBindTexture(GL_TEXTURE_2D_ARRAY_EXT, 0);
	
	bindBuffer();
	glFramebufferTextureEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, m_colorIDs[0], 0);
	glFramebufferTextureEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, m_depthID, 0);
	unbindBuffer();
	
}



void FrameBuffer::automaticallyGenerateMipmaps(int attachment)
{
	assert(m_automaticMipmapsEnabled);
	
	GLenum type = textureType(attachment);
	glBindTexture(type, textureID(attachment));
	glGenerateMipmapEXT(type);
	glBindTexture(type, 0);
}


FrameBuffer::~FrameBuffer()
{
	// unbind this buffer, if bound
	GLint tmpFB;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &tmpFB);
	if (tmpFB == m_ID)
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_prevFrameBuf);
	
	for (int i=0; i < m_maxColorBuffers; i++)
		if (m_colorIDs[i])
			glDeleteTextures(1, &m_colorIDs[i]);
	
	// delete the stencil & depth renderbuffers
	if (m_depthID)
		glDeleteTextures(1, &m_depthID);
	//glDeleteRenderbuffersEXT(1, &m_depthID);
	if (m_stencilID)
		glDeleteRenderbuffersEXT(1, &m_stencilID);
	
	// delete the framebuffer
	glDeleteFramebuffersEXT(1, &m_ID);
	delete [] m_colorIDs;
	delete [] m_colorType; 
}



/*!
	Check to see if the framebuffer 'fb' is complete (i.e., renderable) 
	if fb==NULL, then check the currently bound framebuffer
 */
GLenum
FrameBuffer::checkFramebufferStatus(int printMessage)
{
	GLenum error;
	GLint oldFB = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &oldFB);
	
	// there may be some other framebuffer currently bound...  if so, save it 
	if (oldFB != m_ID)
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_ID);
	
	// check the error status of this framebuffer */
	error = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	
	// if error != GL_FRAMEBUFFER_COMPLETE_EXT, there's an error of some sort 
	if (printMessage)
	{
		switch(error)
		{
			case GL_FRAMEBUFFER_COMPLETE_EXT:
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
				printf("Error!  %s missing a required image/buffer attachment!\n", m_FBOName);
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
				printf("Error!  %s has no images/buffers attached!\n", m_FBOName);
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
				printf("Error!  %s has mismatched image/buffer dimensions!\n", m_FBOName);
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
				printf("Error!  %s's colorbuffer attachments have different types!\n", m_FBOName);
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
				printf("Error!  %s trying to draw to non-attached color buffer!\n", m_FBOName);
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
				printf("Error!  %s trying to read from a non-attached color buffer!\n", m_FBOName);
				break;
			case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
				printf("Error!  %s format is not supported by current graphics card/driver!\n", m_FBOName);
				break;
			default:
				printf("*UNKNOWN ERROR* reported from glCheckFramebufferStatusEXT() for %s!\n", m_FBOName);
				break;
		}
	}
	
	// if this was not the current framebuffer, switch back! 
	if (oldFB != m_ID)
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, oldFB);
	
	return error;
}

/*!
	Attach a texture (colorTexID) to one of the color buffer attachment points 
	This function is not completely general, as it does not allow specification
	of which MIPmap level to draw to (it uses the base, level 0).
 */
int
FrameBuffer::attachColorTexture(GLuint colorTexID, int colorBuffer)
{
	// If the colorBuffer value is valid, then bind the texture to the color buffer.
	if (colorBuffer < m_maxColorBuffers)
	{
		bindBuffer();
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT+colorBuffer, 
								  GL_TEXTURE_2D, colorTexID, 0);
		m_includedBuffers |= GL_COLOR_BUFFER_BIT;
		unbindBuffer();
		if (m_colorIDs[colorBuffer]==0 && colorTexID>0 ) m_numColorAttachments++;
		if (m_colorIDs[colorBuffer]!=0 && colorTexID==0) m_numColorAttachments--;
	}
	else
		return 0;
	m_colorIDs[colorBuffer] = colorTexID;
	return 1;
}


//! Attach a texture (depthTexID) to the depth buffer attachment point.
int
FrameBuffer::attachDepthTexture(GLuint depthTexID)
{
	bindBuffer();
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, 
							  GL_TEXTURE_2D, depthTexID, 0);
	m_depthID = depthTexID;
	m_includedBuffers |= GL_DEPTH_BUFFER_BIT;
	unbindBuffer();
	return 1;
}


//! Attach a texture (stencilTexID) to the stencil buffer attachment point.
int
FrameBuffer::attachStencilTexture(GLuint stencilTexID)
{
	bindBuffer();
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, 
							  GL_TEXTURE_2D, stencilTexID, 0);
	m_stencilID = stencilTexID;
	m_includedBuffers |= GL_STENCIL_BUFFER_BIT;
	unbindBuffer();
	return 1;
}


//! Attach a renderbuffer (colorBufID) to one of the color buffer attachment points.
int
FrameBuffer::attachColorBuffer(GLuint colorBufID, int colorBuffer)
{
	// If the colorBuffer value is valid, then bind the texture to the color buffer.
	if (colorBuffer < m_maxColorBuffers)
	{
		bindBuffer();
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, colorBufID);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGBA, 
		                         m_width, m_height);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT+colorBuffer, 
									 GL_RENDERBUFFER_EXT, colorBufID);
		m_includedBuffers |= GL_COLOR_BUFFER_BIT;
		if (m_colorIDs[colorBuffer]==0 && colorBufID>0 ) m_numColorAttachments++;
		if (m_colorIDs[colorBuffer]!=0 && colorBufID==0) m_numColorAttachments--;
		unbindBuffer();
	}
	else
		return 0;
	m_colorIDs[colorBuffer] = colorBufID;
	return 1;
}

//! Attach a renderbuffer (depthBufID) to the depth buffer attachment point.
int
FrameBuffer::attachDepthBuffer(GLuint depthBufID)
{
	bindBuffer();
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, 
								 GL_RENDERBUFFER_EXT, depthBufID);
	m_includedBuffers |= GL_DEPTH_BUFFER_BIT;
	m_depthID = depthBufID;
	unbindBuffer();
	return 1;
}

//! Attach a renderbuffer (stencilBufID) to the stencil buffer attachment point.
int
FrameBuffer::attachStencilBuffer(GLuint stencilBufID)
{
	bindBuffer();
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, 
								 GL_RENDERBUFFER_EXT, stencilBufID);
	m_includedBuffers |= GL_STENCIL_BUFFER_BIT;
	m_stencilID = stencilBufID;
	unbindBuffer();
	return 1;
}

//! Bind as the current OpenGL FBO.
/*!
	Bind this framebuffer as the current one. Store the old one to reattach
	when we unbind. Also return the ID of the previous framebuffer.
 */
GLuint
FrameBuffer::bindBuffer(void)
{
	GLint tmp;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &tmp);
	m_prevFrameBuf = tmp;
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_ID);
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0, 0, m_width, m_height);
	return m_prevFrameBuf;
}



//! Unbinds this FBO to whatever buffer was attached previously.
/*!
	If for some reason the binding have changed so we're
	no longer the current buffer, DO NOT unbind, return 0.  Else, unbind
	and return 1.
 */
int
FrameBuffer::unbindBuffer(void)
{
	GLint tmpFB;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &tmpFB);
	if (tmpFB != m_ID) return 0;
	glPopAttrib();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_prevFrameBuf);
	m_prevFrameBuf = 0;
	return 1;
}

void
FrameBuffer::drawToColorMipmapLevel(GLuint colorBuffer, GLuint level)
{
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
                              GL_COLOR_ATTACHMENT0_EXT+colorBuffer,
                              GL_TEXTURE_2D, colorTextureID(colorBuffer), level);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, level-1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, level-1);
	
	glBindTexture(GL_TEXTURE_2D, colorTextureID(colorBuffer));
	glEnable(GL_TEXTURE_2D);
}

void
FrameBuffer::doneDrawingMipmapLevels(void)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1000);
}


void
FrameBuffer::blitFramebuffer(int attachment)
{
	glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, textureID(attachment));
	glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, 0);
	glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height,
					  GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, 0);
	glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, 0);
}

void
FrameBuffer::displayAsFullScreenTexture(int attachment, float lod)
{
	int minFilter, magFilter;
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0,1,0,1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID(attachment));
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint*)&minFilter);
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint*)&magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, lod > 0 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, lod > 0 ? GL_LINEAR : GL_LINEAR);
	if (lod > 0)
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, lod);
	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);
	
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0,0);	glVertex2f(0,0);
		glTexCoord2f(1,0);	glVertex2f(1,0);
		glTexCoord2f(1,1);	glVertex2f(1,1);
		glTexCoord2f(0,1);	glVertex2f(0,1);
	}
	glEnd();
	
	if (lod > 0)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopAttrib();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}


void
FrameBuffer::displayAlphaAsFullScreenTexture(int attachment, float lod)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0,1,0,1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ZERO, GL_SRC_ALPHA);
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID(attachment));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, lod > 0 ? GL_NEAREST_MIPMAP_NEAREST : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, lod > 0 ? GL_NEAREST_MIPMAP_NEAREST : GL_LINEAR);
	if (lod > 0) glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, lod);
	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0,0);	glVertex2f(0,0);
	glTexCoord2f(1,0);	glVertex2f(1,0);
	glTexCoord2f(1,1);	glVertex2f(1,1);
	glTexCoord2f(0,1);	glVertex2f(0,1);
	glEnd();
	if (lod > 0) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopAttrib();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

GLuint
FrameBuffer::textureID(int attachment)
{
	if (attachment == FBO_DEPTH) return m_depthID;
	if (attachment == FBO_STENCIL) return m_stencilID;
	return colorTextureID(attachment - FBO_COLOR0);
}

GLenum
FrameBuffer::textureType(int attachment)
{
	if (attachment == FBO_DEPTH) return m_depthType;
	if (attachment == FBO_STENCIL) return m_stencilType;
	assert(attachment-FBO_COLOR0 < m_maxColorBuffers);
	return m_colorType[attachment-FBO_COLOR0];
}


/*!
	This resizes a framebuffer object that already exists and has textures
	already associated with it.
*/
void
FrameBuffer::resizeExistingFBO(int newWidth, int newHeight)
{
	GLint format;
	
	// Resize the color buffers
	for (int i = 0; i < m_maxColorBuffers; i++)
	{
		if (m_colorIDs[i] > 0)
		{
			glBindTexture(m_colorType[i], m_colorIDs[i]);
			glGetTexLevelParameteriv(m_colorType[i], 0, GL_TEXTURE_INTERNAL_FORMAT, &format);	
			if (m_colorType[i] == GL_TEXTURE_2D)
				glTexImage2D(GL_TEXTURE_2D, 0, format, newWidth, newHeight, 0, GL_RGBA, GL_FLOAT, NULL);
			else if (m_colorType[i] == GL_TEXTURE_2D_ARRAY_EXT)
				glTexImage3D(GL_TEXTURE_2D_ARRAY_EXT, 0, format, newWidth, newHeight, m_depth, 0, GL_RGBA, GL_FLOAT, NULL);
		}
	}	
	
	if (m_depthID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, m_depthID);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &format);	
		glTexImage2D(GL_TEXTURE_2D, 0, format, newWidth, newHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}

	if (m_stencilID > 0)
	{
		printf("**** Error:  Called FrameBuffer::resizeExistingFBO() on FBO with stencil\n");
		printf("             buffer.  Resizing FBOs with stencil bufs is not supported yet!\n");
		exit(0);
	}
	
	glBindTexture(GL_TEXTURE_2D, 0);
	m_width = newWidth; 
	m_height = newHeight;
	
	checkFramebufferStatus(1);
}

