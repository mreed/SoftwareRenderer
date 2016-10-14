#include "stdafx.h"
#include "GLFWDisplay.h"
#include <Windows.h>
#include "..\glext.h"
#include "..\glinfo.h"


void GLFWDisplay::Init(int width, int height)
{
	_width = width;
	_height = height;

	glfwSetErrorCallback(&GLFWDisplay::error_callback);

	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	_window = glfwCreateWindow(_width, _height, "Software Render", NULL, NULL);
	if (!_window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(_window,&GLFWDisplay::key_callback);

	glfwMakeContextCurrent(_window);
	glewExperimental = GL_TRUE;
	
	glewInit();
	if (glewIsSupported("GL_VERSION_3_3"))
	{
		std::cout << " GLEW Version is 3.3\n";

	}
	else
	{
		std::cout << "GLEW Version 3.3 not supported\n";
	}
	glInfo glInfo;
	glInfo.getInfo();
	glInfo.printSelf();

	bool (__stdcall *wglSwapIntervalEXT)(int) =(bool(__stdcall *)( int)) GetGLFuncAddress("wglSwapIntervalEXT");
	if (wglSwapIntervalEXT != nullptr)
		wglSwapIntervalEXT(0);
	_game->Init(_width,_height);

}

void GLFWDisplay::MainLoop()
{
	while (!glfwWindowShouldClose(_window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		_game->Update();
		_game->Render();
		SetWindowFPS();
		glfwSwapBuffers(_window);
		
		glfwPollEvents();
	}
	glfwDestroyWindow(_window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void GLFWDisplay::SetWindowFPS()
{
	// Measure speed
	double currentTime = glfwGetTime();
	_frames++;

	if (currentTime - _lastTime >= 1.0) { // If last cout was more than 1 sec ago
		char title[256];
		title[255] = '\0';

		snprintf(title, 255, "[FPS: %3.2f]",_frames/(currentTime - _lastTime));

		glfwSetWindowTitle(_window, title);

		_frames = 0;
		_lastTime = currentTime;
	}
}

void * GLFWDisplay::GetGLFuncAddress(const char * name)
{
	void *p = (void *)wglGetProcAddress(name);
	if (p == 0 ||
		(p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
		(p == (void*)-1))
	{
		HMODULE module = LoadLibraryA("opengl32.dll");
		p = (void *)GetProcAddress(module, name);
	}

	return p;
}


GLFWDisplay::GLFWDisplay()
{
	_game = new Game();
}


GLFWDisplay::~GLFWDisplay()
{
	if (_game != NULL)
		delete _game;
	_game = NULL;
}
