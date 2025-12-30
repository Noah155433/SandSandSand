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

void OpenGL::createVBO(unsigned int& VBO, float* vertexArray, int size)
{

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, vertexArray, GL_STATIC_DRAW);

}

void OpenGL::createVAO(unsigned int& VAO, int* attribArray, int attribSize)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	int stride = 0;
	for (int i = 0; i < attribSize; ++i)
	{
		stride += attribArray[i];
	}

	int offset = 0;

	for (int i = 0; i < attribSize; ++i)
	{
		glVertexAttribPointer(i, attribArray[i], GL_FLOAT, GL_FALSE, stride * sizeof(float),
			(void*)(sizeof(float) * offset));
		glEnableVertexAttribArray(i);
		offset += attribArray[i];
	}

	
}

void OpenGL::createTexture(unsigned int& texture, GLenum format, int width, int height, GLenum colors, const void* data)
{

	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_R8,
		width,
		height,
		0,
		GL_RED,
		GL_UNSIGNED_BYTE,
		data
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

}

void OpenGL::bindTexture(int index, unsigned int& texture, Shader shader)
{
	glActiveTexture(index);
	glBindTexture(GL_TEXTURE_2D, texture);
	shader.setInt("texture" + (char)(index + 48), index);
}
