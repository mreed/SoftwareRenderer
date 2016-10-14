#pragma once
#include "Graphics\Shader.h"
#include "Bitmap.h"
#include "PBOTexture.h"
class Game
{
public:
	void Init(int width, int height);
	void Update();
	void Render();
	Game();
	~Game();
private:
	Shader* _shader;
	GLuint _vao, _vbo,_ebo, _texture;
	Bitmap* _bitmap;
	int _width, _height;
	PBOTexture * _pboTexture;
};

