#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"

#include <iostream>
#include <fstream>
#include <string>
#include <random>

void processInput(GLFWwindow* window);
void cursorPosCallback(GLFWwindow* window, double xPos, double yPos);

void createVBO(unsigned int& VBO, float* vertexArray, int vertexSize);
void createVAO(unsigned int& VAO, int* VAOAttribs, int attribsSize);

void createTexture(unsigned int& texture, GLenum format, int width, int height, GLenum colors, const void* data);
void loadImageData(const char* fileName, int& width, int& height, int& nrChannels, unsigned char*& data);
void bindTexture(int index, unsigned int texture, Shader& shader);


void startGame();

glm::vec2 mousePos = glm::vec2(0.0f);
bool leftButtonPressed = false;

int width, height;

enum GameState
{
	GAMESTATE_MENU,
	GAMESTATE_SIMULATION,
	GAMESTATE_OPTIONS
};

class UIElement
{
public:
	UIElement();
	~UIElement();

	void renderButton();
	void setMinMax();

	glm::vec2 m_size;
	glm::vec2 m_position;
	unsigned int m_texture;
	unsigned int m_VAO;
	Shader* m_shader;
	int m_index;
	void (*m_func)();
	GameState m_buttonState;
	int m_minX;
	int m_minY;
	int m_maxX;
	int m_maxY;

};

UIElement::UIElement()
{
	m_size = glm::vec2(1.0f);
	m_position = glm::vec2(1.0f);
	m_texture = 0;
	m_VAO = 0;
	m_shader = 0;
	m_index = 0;
	m_func = 0;
	m_buttonState = GAMESTATE_MENU;

}

UIElement::~UIElement()
{
}

void UIElement::renderButton()
{
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(m_position, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(m_size, 0.0f));
	m_shader->use();
	bindTexture(0, m_texture, *m_shader);
	m_shader->setMat4("model", modelMatrix);
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void UIElement::setMinMax()
{
	glm::vec4 buttonVertices[] =
	{
		glm::vec4(-1, -1, 0, 1),
		glm::vec4(1, -1, 0, 1),
		glm::vec4(-1, 1, 0, 1),
		glm::vec4(1, 1, 0, 1)
	};

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(m_position, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(m_size, 0.0f));

	glm::vec4 startButtonWorldCorner[4];

	for (int i = 0; i < 4; ++i)
	{
		startButtonWorldCorner[i] = modelMatrix * buttonVertices[i];
	}

	glm::vec2 startButtonScreenCorner[4];

	for (int i = 0; i < 4; ++i)
	{
		float x = (startButtonWorldCorner[i].x * 0.5f + 0.5f) * width;
		float y = (startButtonWorldCorner[i].y * 0.5f + 0.5f) * height;

		startButtonScreenCorner[i] = { x, y };
	}

	m_minX = startButtonScreenCorner[0].x;
	m_minY = startButtonScreenCorner[1].y;
	m_maxX = startButtonScreenCorner[3].x;
	m_maxY = startButtonScreenCorner[2].y;
}

GameState gamestate = GAMESTATE_MENU;

void checkButtons(UIElement* buttons, int size);

int spawnColor = 2;

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

	GLFWwindow* window = glfwCreateWindow(width, height, "Sand Sand Sand", glfwGetPrimaryMonitor(), NULL);
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

	stbi_set_flip_vertically_on_load(true);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

	unsigned int VBO, VAO, UIVBO, UIVAO, BUTTONVBO, BUTTONVAO;
	int VaoAttribs[] = { 3, 2 };
	createVBO(VBO, squareVertices, 3 * 6);
	createVAO(VAO, VaoAttribs, 1);
	createVBO(UIVBO, uiRectVertices, 5 * 6);
	createVAO(UIVAO, VaoAttribs, 2);
	createVBO(BUTTONVBO, uiRectVertices, 5 * 6);
	createVAO(BUTTONVAO, VaoAttribs, 2);

	unsigned int FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	GLenum attachments[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachments);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	unsigned int tex1, tex2, tex3, tex4, sandNoise;
	int texWidth, texHeight, nrChannels;
	unsigned char* data{};
	createTexture(tex1, GL_R8UI, width, height, GL_RED_INTEGER, (void*)sand.data());
	createTexture(tex2, GL_R8UI, width, height, GL_RED_INTEGER, (void*)sand.data());
	loadImageData("SandSandSandTitle.png", texWidth, texHeight, nrChannels, data);
	createTexture(tex3, GL_RGBA, texWidth, texHeight, GL_RGBA, data);
	stbi_image_free(data);
	loadImageData("SandNoise.png", texWidth, texHeight, nrChannels, data);
	createTexture(sandNoise, GL_RGB, texWidth, texHeight, GL_RGB, data);
	stbi_image_free(data);
	loadImageData("StartButton.png", texWidth, texHeight, nrChannels, data);
	createTexture(tex4, GL_RGBA, texWidth, texHeight, GL_RGBA, data);
	stbi_image_free(data);
	
	glfwSetCursorPosCallback(window, cursorPosCallback);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	UIElement startButton;
	startButton.m_position = glm::vec2(0.0f);
	startButton.m_size = glm::vec2(0.25f, 0.2f);
	startButton.m_texture = tex4;
	startButton.m_VAO = BUTTONVAO;
	startButton.m_shader = &uiShader;
	startButton.m_index = 1;
	startButton.m_func = startGame;
	startButton.m_buttonState = GAMESTATE_MENU;
	startButton.setMinMax();

	UIElement Title;
	Title.m_position = glm::vec2(0.0f, 0.7f);
	Title.m_size = glm::vec2(0.5f, 0.2f);
	Title.m_texture = tex3;
	Title.m_VAO = UIVAO;
	Title.m_shader = &uiShader;
	Title.m_index = 0;
	Title.m_func = nullptr;
	Title.m_buttonState = GAMESTATE_MENU;
	Title.setMinMax();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glfwSwapBuffers(window);

		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 TitleTransform = glm::mat4(1.0f);
		glm::mat4 startButtonTransform = glm::mat4(1.0f);
		UIElement buttons[] = { startButton };

		switch (gamestate)
		{
		case GAMESTATE_MENU:	
			Title.renderButton();
			startButton.renderButton();
			checkButtons(buttons, 1);
			break;
		case GAMESTATE_SIMULATION:
			simShader.use();
			bindTexture(0, tex1, simShader);
			bindTexture(1, sandNoise, simShader);
			simShader.setVec2("screenSize", glm::vec2(width, height));
			simShader.setVec2("mousePos", mousePos);
			simShader.setBool("leftButtonPressed", leftButtonPressed);
			simShader.setInt("spawnColor", spawnColor);
			glBindFramebuffer(GL_FRAMEBUFFER, FBO);
			glBindVertexArray(VAO);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex2, 0);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			std::swap(tex1, tex2);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			shader.use();
			bindTexture(0, tex1, shader);
			shader.setVec2("screenSize", glm::vec2(width, height));
			bindTexture(1, sandNoise, shader);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			break;
		default:
			break;
		}

	}

}

void createVBO(unsigned int& VBO, float* vertexArray, int vertexSize)
{
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexSize, vertexArray, GL_STATIC_DRAW);
}

void createVAO(unsigned int& VAO, int* VAOAttribs, int attribsSize)
{

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	int stride = 0;
	for (int i = 0; i < attribsSize; ++i)
	{
		stride += VAOAttribs[i];
	}

	int offset = 0;

	for (int i = 0; i < attribsSize; ++i)
	{
		glVertexAttribPointer(i, VAOAttribs[i], GL_FLOAT, GL_FALSE, stride * sizeof(float),
			(void*)(sizeof(float) * offset));
		glEnableVertexAttribArray(i);
		offset += VAOAttribs[i];
	}


}

void createTexture(unsigned int& texture, GLenum format, int width, int height, GLenum colors, const void* data)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        format,
        width,
        height,
        0,
        colors,
        GL_UNSIGNED_BYTE,
        data
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void loadImageData(const char* fileName, int& width, int& height, int& nrChannels, unsigned char*& data)
{
	data = stbi_load(fileName, &width, &height, &nrChannels, 0);
	
}

void bindTexture(int index, unsigned int texture, Shader& shader)
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, texture);

	std::string uniformName = "texture" + std::to_string(index + 1);
	shader.setInt(uniformName.c_str(), index);
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
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		spawnColor = 2;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		spawnColor = 4;
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		spawnColor = 6;
	}

	leftButtonPressed = false;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		leftButtonPressed = true;
	}
}

void checkButtons(UIElement* buttons, int size)
{
	
	for (int i = 0; i < size; ++i)
	{
		float minX = buttons[i].m_minX;
		float minY = buttons[i].m_minY;
		float maxX = buttons[i].m_maxX;
		float maxY = buttons[i].m_maxY;

		if (leftButtonPressed && mousePos.x > minX && mousePos.x < maxX && mousePos.y > minY && mousePos.y < maxY)
		{
			buttons[i].m_func();
			std::cout << buttons[i].m_index << std::endl;
		}

		std::cout << buttons[i].m_minX << " | " << buttons[i].m_minY << " | " << buttons[i].m_maxX << " | " << buttons[i].m_maxY << std::endl;

	}

}

void startGame()
{
	gamestate = GAMESTATE_SIMULATION;
}