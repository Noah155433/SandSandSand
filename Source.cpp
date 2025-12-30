#include "OpenGL.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"

void processInput(GLFWwindow* window);
void cursorPosCallback(GLFWwindow* window, double xPos, double yPos);

glm::vec2 mousePos = glm::vec2(0.0f);
bool leftButtonPressed = false;

bool ShouldSimulate = true;

int width, height;

int main()
{
	
	OpenGL openGL;
	GLFWwindow* window;
	openGL.Initialize(width, height, window);

	std::vector<unsigned char> sand;
	sand.resize(width * height);

	Shader shader("VertexShader.vert", "FragmentShader.frag");
	Shader simShader("VertexShader.vert", "SimulationShader.frag");
	Shader uiShader("UIVertex.vert", "UIFragment.frag");

	float squareVertices[] =
	{
		-1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f
	};

	float uiRectVertices[] =
	{
		-1.0f, -1.0f, 0.0f, 0.0, 0.0,
		-1.0f,  1.0f, 0.0f, 0.0, 1.0,
		 1.0f, -1.0f, 0.0f, 1.0, 0.0,
		-1.0f,  1.0f, 0.0f, 0.0, 1.0,
		 1.0f,  1.0f, 0.0f, 1.0, 1.0,
		 1.0f, -1.0f, 0.0f, 1.0, 0.0
	};

	unsigned int VBO, VAO, UIVBO, UIVAO;
	int VaoAttribs[] = { 3, 2 };
	openGL.createVBO(VBO, squareVertices, 3 * 6);
	openGL.createVAO(VAO, VaoAttribs, 1);
	openGL.createVBO(UIVBO, uiRectVertices, 5 * 6);
	openGL.createVAO(UIVAO, VaoAttribs, 2);

	unsigned int FBO;
	glGenFramebuffers(1, &FBO);

	GLuint tex1, tex2, tex3;
	int texWidth, texHeight;
	unsigned char* data{};
	openGL.createTexture(tex1, GL_R8, width, height, GL_RED, (void*)sand.data());
	openGL.createTexture(tex2, GL_R8, width, height, GL_RED, (void*)sand.data());
	openGL.loadImageData("SandSandSandTitle.png", texWidth, texHeight, data);
	openGL.createTexture(tex3, GL_RGBA, texWidth, texHeight, GL_RGBA, data);
	
	glfwSetCursorPosCallback(window, cursorPosCallback);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glfwSwapBuffers(window);

		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 TitleTransform = glm::mat4(1.0f);
		TitleTransform = glm::translate(TitleTransform, glm::vec3(0.0, 0.7, 0.0));
		TitleTransform = glm::scale(TitleTransform, glm::vec3(0.5f, 0.2f, 1.0f));

		if (!ShouldSimulate)
		{
			openGL.bindTexture(0, tex3, uiShader);
			uiShader.use();
			uiShader.setMat4("model", TitleTransform);
			glBindVertexArray(UIVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		if (ShouldSimulate)
		{
			openGL.bindTexture(0, tex1, simShader);
			simShader.use();
			simShader.setVec2("screenSize", glm::vec2(width, height));
			simShader.setVec2("mousePos", mousePos);
			simShader.setBool("leftButtonPressed", leftButtonPressed);
			glBindFramebuffer(GL_FRAMEBUFFER, FBO);
			glBindVertexArray(VAO);
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