#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class OpenGL
{
public:
	OpenGL();

	void Initialize(int&, int&, GLFWwindow*&);
};

