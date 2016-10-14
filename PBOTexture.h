#pragma once
#include <GL\GL.h>
#include "glInfo.h"
class PBOTexture
{
public:
	PBOTexture();
	~PBOTexture();
	void Init(int width, int height,glInfo& glInfo);
	void Update();
	void updatePixels(GLubyte* dst, int size)
	{
		static int color = 0;

		if (!dst)
			return;

		int* ptr = (int*)dst;

		// copy 4 bytes at once
		for (int i = 0; i < 200; ++i)
		{
			for (int j = 0; j < 200; ++j)
			{
				*ptr = color;
				++ptr;
			}
			color += 257;   // add an arbitary number (no meaning)
		}
		++color;            // scroll down
	}
	GLuint GetTextureId() { return _textureId; }
private:
	int _width, _height, _channelCount;
	GLenum _pixelFormat;
	GLuint _pboIds[2],_textureId;
	GLubyte* _imageData;
	int _dataSize;

};

