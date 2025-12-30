#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "shader_s.h"
#include "stb_image.h"

class OpenGL
{
public:
	OpenGL();

	void Initialize(int&, int&, GLFWwindow*&);
	void createVBO(unsigned int&, float*, int);
	void createVAO(unsigned int&, int*, int);
	void createTexture(unsigned int&, GLenum, int, int, GLenum, const void*);
	void bindTexture(int, unsigned int&, Shader);
	void loadImageData(const char*, int&, int&, unsigned char*&);
};