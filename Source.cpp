#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"

#include <iostream>
#include <vector>

void processInput(GLFWwindow* window);
void cursorPosCallback(GLFWwindow* window, double xPos, double yPos);

glm::vec2 mousePos = glm::vec2(0.0f);
bool leftButtonPressed = false;

int width, height;

int main()
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

	GLFWwindow* window = glfwCreateWindow(width, height, "SandSandSand", glfwGetPrimaryMonitor(), NULL);
	if (window == NULL)
	{
		std::cout << "Window creation failed" << std::endl;
	}

	glfwGetFramebufferSize(window, &width, &height);

	std::vector<unsigned char> sand;
	sand.resize(width * height);

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Could not load GLL loader" << std::endl;
	}

	glViewport(0, 0, 2560, 1440);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	Shader shader("VertexShader.vert", "FragmentShader.frag");
	Shader simShader("VertexShader.vert", "SimulationShader.frag");

	float cubeVertices[] =
	{
		-1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f
	};

	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int FBO;
	glGenFramebuffers(1, &FBO);

	GLuint tex1, tex2;
	glGenTextures(1, &tex1);
	glGenTextures(1, &tex2);

	for (GLuint tex : {tex1, tex2})
	{
		glBindTexture(GL_TEXTURE_2D, tex);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_R8,
			width,
			height,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			(void*)sand.data()
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	
	glfwSetCursorPosCallback(window, cursorPosCallback);

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, tex1);
	shader.setInt("texture1", 0);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glfwSwapBuffers(window);

		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT);

		simShader.use();
		simShader.setVec2("screenSize", glm::vec2(width, height));
		simShader.setVec2("mousePos", mousePos);
		simShader.setBool("leftButtonPressed", leftButtonPressed);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex2, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		std::swap(tex1, tex2);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, tex1);
		shader.use();
		shader.setVec2("screenSize", glm::vec2(width, height));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);


	}

}

void cursorPosCallback(GLFWwindow* window, double xPos, double yPos)
{
	mousePos = glm::vec2(xPos, height - yPos);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	leftButtonPressed = false;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		leftButtonPressed = true;
	}
}