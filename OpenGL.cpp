#include "OpenGL.h"

OpenGL::OpenGL()
{
}

void OpenGL::Initialize(int& width, int& height, GLFWwindow*& window)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

	width = glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
	height = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;

	window = glfwCreateWindow(width, height, "SandSandSand", glfwGetPrimaryMonitor(), NULL);
	if (window == NULL)
	{
		std::cout << "Window creation failed" << std::endl;
	}

	glfwGetFramebufferSize(window, &width, &height);

	

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Could not load GLL loader" << std::endl;
	}

	glViewport(0, 0, width, height);
}
