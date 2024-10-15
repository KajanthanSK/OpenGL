#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "stb_image.h"

float vertices[] =
{
	/* Position */		/* Color */			/* TextureCoords */
	-0.5f,-0.5f,0.0f,	1.0f,0.0f,0.0f,		0.0f, 0.0f,
	 0.5f,-0.5f,0.0f,	0.0f,1.0f,0.0f,		1.0f, 0.0f,
	 0.5f, 0.5f,0.0f,	0.0f,0.0f,1.0f,		1.0f, 1.0f,

	 0.5f, 0.5f,0.0f,	0.0f,0.0f,1.0f,		1.0f, 1.0f,
	-0.5f, 0.5f,0.0f,	1.0f,0.0f,1.0f,		0.0f, 1.0f,
	-0.5f,-0.5f,0.0f,	1.0f,0.0f,0.0f,		0.0f, 0.0f,
};

// Prototype
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void userInput(GLFWwindow* window);
void mouseCursorPosition(GLFWwindow* window, double xPos, double yPos);
void mouseScrollPosition(GLFWwindow* window, double xOffset, double yOffset);
unsigned int loadTexture(const char* texturePath);

glm::mat4 model;
glm::vec3 myPos = glm::vec3(0.0f);

int main()
{
	/* Initialize GLFW */
	glfwInit();

	/* Initialize Version 3.3 */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a window */
	GLFWwindow* window = glfwCreateWindow(1000, 800, "OpenGL Window", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to Create a window\n";
		glfwTerminate();
	}

	/* The most important part */
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, mouseCursorPosition);
	glfwSetScrollCallback(window, mouseScrollPosition);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize Glew\n";
		glfwTerminate();
	}

	/* Buffers */
	unsigned int VBO, VAO;

	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	/* Position Attribute */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/* Color Attribute*/
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	/* Texture Coordinates Attribute*/
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	/* Texture */
	stbi_set_flip_vertically_on_load(true);
	GLuint containerTexture = loadTexture("container.jpg");
	GLuint faceTexture = loadTexture("awesomeface.png");

	/* Shader */
	Shader shader("vertexShader.glsl", "fragmentShader.glsl");
	shader.use();
	shader.setInt("containerTexture", 0);
	shader.setInt("faceTexture", 1);

	while (!glfwWindowShouldClose(window))
	{
		/* Update */
		userInput(window);
		float time = glfwGetTime();

		float xValue = std::sin(time) / 2.0f + 0.5f; 
		float yValue = std::sin(time) / 2.0f + 0.5f; 
		float zValue = std::sin(time) / 2.0f + 0.5f; 

		// vector
		glm::vec3 myVector;
		myVector.x = xValue;
		myVector.y = yValue;
		myVector.z = 0.31f;

		shader.setVec3v("color", myVector);
		shader.setFloat("alpha", yValue);

		// matrix
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(myPos));
		shader.setMat4("model", model);

		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, containerTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, faceTexture);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		
		glfwPollEvents();
	}

	/* Clear */
	glfwDestroyWindow(window);
	glfwTerminate();
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void userInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_TRUE)
		myPos.y += 0.0005f;
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_TRUE)
		myPos.y -= 0.0005f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_TRUE)
		myPos.x += 0.0005f;
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_TRUE)
		myPos.x -= 0.0005f;
}

void mouseCursorPosition(GLFWwindow* window, double xPos, double yPos)
{
	std::cout << "mouse moving\n";
}

void mouseScrollPosition(GLFWwindow* window, double xOffset, double yOffset)
{
	std::cout << "mouse scroll\n";
}

unsigned int loadTexture(const char* texturePath)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	/* Filter Options */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

	if (data)
	{
		// it's a better way to see that what our file is like png, jpg or jpeg ?
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		if (nrChannels == 3) // jpg file
			format = GL_RGB;
		if (nrChannels == 4) // png file
			format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture\n";
	}

	stbi_image_free(data);

	return textureID;
}