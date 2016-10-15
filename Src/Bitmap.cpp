#include "stdafx.h"
#include "Bitmap.h"
#include <string.h>



Bitmap::Bitmap(int width, int height)
{
	_width = width;
	_height = height;
	_buffer = new unsigned char[width * height * sizeof(int)];
}

Bitmap::~Bitmap()
{
	if (_buffer != NULL)
		delete[] _buffer;
	_buffer = NULL;
}

void Bitmap::Clear(unsigned char color)
{
	memset(_buffer, color, Size());
}

void Bitmap::DrawPixel(int x, int y, int color)
{
	int index = (y * _width + x) * sizeof(int);
	*((int*)(_buffer + index)) = color;
	//*((int*)&_buffer[(y*_width + x) * sizeof(int)]) = color;
}
