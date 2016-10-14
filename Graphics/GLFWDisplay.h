#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "..\Game.h"


class GLFWDisplay
{
public:
	static GLFWDisplay& GetInstance()
	{
		static GLFWDisplay instance;
		return instance;
	}
	void Init(int width, int height);
	void MainLoop();
	void SetWindowFPS();
	void* GetGLFuncAddress(const char* name);
	static void error_callback(int error, const char* description)
	{
		fprintf(stderr, "Error %s\n", description);
	}
	static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
	}
private:
	GLFWDisplay();
	~GLFWDisplay();
	GLFWDisplay(GLFWDisplay const&);
	void operator=(GLFWDisplay const&);
	GLFWwindow* _window;
	int _width;
	int _height;
	int _frames;
	double _lastTime;
	Game* _game;
	
};

