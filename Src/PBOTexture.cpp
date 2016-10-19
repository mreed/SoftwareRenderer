#include "stdafx.h"
#include "PBOTexture.h"
#ifdef _WIN32
#include <Windows.h>

// function pointers for PBO Extension
// Windows needs to get function pointers from ICD OpenGL drivers,
// because opengl32.dll does not support extensions higher than v1.1.

PFNGLGENBUFFERSARBPROC pglGenBuffersARB = 0;                     // VBO Name Generation Procedure
PFNGLBINDBUFFERARBPROC pglBindBufferARB = 0;                     // VBO Bind Procedure
PFNGLBUFFERDATAARBPROC pglBufferDataARB = 0;                     // VBO Data Loading Procedure
PFNGLBUFFERSUBDATAARBPROC pglBufferSubDataARB = 0;               // VBO Sub Data Loading Procedure
PFNGLDELETEBUFFERSARBPROC pglDeleteBuffersARB = 0;               // VBO Deletion Procedure
PFNGLGETBUFFERPARAMETERIVARBPROC pglGetBufferParameterivARB = 0; // return various parameters of VBO
PFNGLMAPBUFFERARBPROC pglMapBufferARB = 0;                       // map VBO procedure
PFNGLUNMAPBUFFERARBPROC pglUnmapBufferARB = 0;                   // unmap VBO procedure
#define glGenBuffersARB           pglGenBuffersARB
#define glBindBufferARB           pglBindBufferARB
#define glBufferDataARB           pglBufferDataARB
#define glBufferSubDataARB        pglBufferSubDataARB
#define glDeleteBuffersARB        pglDeleteBuffersARB
#define glGetBufferParameterivARB pglGetBufferParameterivARB
#define glMapBufferARB            pglMapBufferARB
#define glUnmapBufferARB          pglUnmapBufferARB
#endif

PBOTexture::PBOTexture()
{

}


PBOTexture::~PBOTexture()
{
	if (_imageData != NULL)
		delete[] _imageData;
	_imageData = NULL;
	glDeleteTextures(1, &_textureId);
	glDeleteBuffersARB(2, _pboIds);
}
void PBOTexture::Init(int width, int height,glInfo& glInfo)
{
	_width = width;
	_height = height;
	_channelCount = 4;
	_pixelFormat = GL_BGRA;
	_dataSize = _width * _height * _channelCount;
	_imageData = new GLubyte[_dataSize];
	memset(_imageData, 0, _dataSize);

#ifdef _WIN32
	// check PBO is supported by your video card
	if (glInfo.isExtensionSupported("GL_ARB_pixel_buffer_object"))
	{
		// get pointers to GL functions
		glGenBuffersARB = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
		glBindBufferARB = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
		glBufferDataARB = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");
		glBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC)wglGetProcAddress("glBufferSubDataARB");
		glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffersARB");
		glGetBufferParameterivARB = (PFNGLGETBUFFERPARAMETERIVARBPROC)wglGetProcAddress("glGetBufferParameterivARB");
		glMapBufferARB = (PFNGLMAPBUFFERARBPROC)wglGetProcAddress("glMapBufferARB");
		glUnmapBufferARB = (PFNGLUNMAPBUFFERARBPROC)wglGetProcAddress("glUnmapBufferARB");

		// check once again PBO extension
		if (glGenBuffersARB && glBindBufferARB && glBufferDataARB && glBufferSubDataARB &&
			glMapBufferARB && glUnmapBufferARB && glDeleteBuffersARB && glGetBufferParameterivARB)
		{
			//pboSupported = true;
			//pboMode = 1;    // using 1 PBO
			std::cout << "Video card supports GL_ARB_pixel_buffer_object." << std::endl;
		}
		else
		{
			exit(EXIT_FAILURE);
			std::cout << "Video card does NOT support GL_ARB_pixel_buffer_object." << std::endl;
		}
	}
#endif
	
	glGenTextures(1, &_textureId);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, _pixelFormat, GL_UNSIGNED_BYTE, (GLvoid*)_imageData);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenBuffersARB(2, _pboIds);
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, _pboIds[0]);
	glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, _dataSize, 0, GL_STREAM_DRAW_ARB);
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, _pboIds[1]);
	glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, _dataSize, 0, GL_STREAM_DRAW_ARB);
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);

}

void PBOTexture::Update()
{
	static int index = 0;
	int nextIndex = 0;

	index = (index + 1) % 2;
	nextIndex = (index + 1) % 2;

	// bind the texture and PBO
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, _pboIds[index]);

	// copy pixels from PBO to texture object
	// Use offset instead of ponter.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, _pixelFormat, GL_UNSIGNED_BYTE, 0);
	// bind PBO to update pixel values
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, _pboIds[nextIndex]);

	// map the buffer object into client's memory
	// Note that glMapBufferARB() causes sync issue.
	// If GPU is working with this buffer, glMapBufferARB() will wait(stall)
	// for GPU to finish its job. To avoid waiting (stall), you can call
	// first glBufferDataARB() with NULL pointer before glMapBufferARB().
	// If you do that, the previous data in PBO will be discarded and
	// glMapBufferARB() returns a new allocated pointer immediately
	// even if GPU is still working with the previous data.
	glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, _dataSize, 0, GL_STREAM_DRAW_ARB);
	GLubyte* ptr = (GLubyte*)glMapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, GL_WRITE_ONLY_ARB);
	if (ptr)
	{
		// update data directly on the mapped buffer
		updatePixels(ptr, _dataSize);
		glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB); // release pointer to mapping buffer
	}

	// it is good idea to release PBOs with ID 0 after use.
	// Once bound with 0, all pixel operations behave normal ways.
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
}
