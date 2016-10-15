#include "../stdafx.h"
#include "GLFWDisplay.h"

#include "../glext.h"
#include "../glinfo.h"

#ifdef _WIN32
#include <Windows.h>

typedef BOOL(WINAPI * PFNWGLSWAPINTERVALEXTPROC) (int interval);
typedef int (WINAPI * PFNWGLGETSWAPINTERVALEXTPROC) (void);
PFNWGLSWAPINTERVALEXTPROC pwglSwapIntervalEXT = 0;
PFNWGLGETSWAPINTERVALEXTPROC pwglGetSwapIntervalEXT = 0;
#define wglSwapIntervalEXT      pwglSwapIntervalEXT
#define wglGetSwapIntervalEXT   pwglGetSwapIntervalEXT
#endif


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
	
	_glInfo.getInfo();
	_glInfo.printSelf();
#ifdef _WIN32
	if (_glInfo.isExtensionSupported("WGL_EXT_swap_control"))
	{
		// get pointers to WGL functions
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
		wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
		if (wglSwapIntervalEXT && wglGetSwapIntervalEXT)
		{
			// disable v-sync
			wglSwapIntervalEXT(0);
			std::cout << "Video card supports WGL_EXT_swap_control." << std::endl;
		}
	}
#endif

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
