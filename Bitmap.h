#pragma once
//#define byte unsigned char
class Bitmap
{
public:
	Bitmap(int width, int height);
	~Bitmap();
	void Clear(unsigned char color);
	void DrawPixel(int x, int y, int color);
	int GetWidth() { return _width; }
	int GetHeight() { return _height; }
	unsigned char* GetData() { return _buffer; }
	int Size() { return _width * _height * sizeof(int); }
private:
	int _width, _height;
	unsigned char* _buffer;
};

