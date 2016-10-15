// SR2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main()
{
	GLFWDisplay& display = GLFWDisplay::GetInstance();

	display.Init(800,600);
	display.MainLoop();
    return 0;
}

