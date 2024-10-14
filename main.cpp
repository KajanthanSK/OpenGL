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
	/* Position */		/* Color */
	-0.5f,-0.5f,0.0f,	1.0f,0.0f,0.0f,
	 0.5f,-0.5f,0.0f,	0.0f,1.0f,0.0f,
	 0.5f, 0.5f,0.0f,	0.0f,0.0f,1.0f,

	 0.5f, 0.5f,0.0f,	0.0f,0.0f,1.0f,
	-0.5f, 0.5f,0.0f,	0.0f,0.0f,1.0f,
	-0.5f,-0.5f,0.0f,	1.0f,0.0f,0.0f,
};

// Prototype
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void userInput(GLFWwindow* window);
void mouseCursorPosition(GLFWwindow* window, double xPos, double yPos);
void mouseScrollPosition(GLFWwindow* window, double xOffset, double yOffset);

glm::mat4 model;
glm::vec3 myPos = glm::vec3(1.0f);

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

	/* Shader */
	Shader shader("vertexShader.glsl", "fragmentShader.glsl");

	/* Buffers */
	unsigned int VBO, VAO;

	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	/* Position Attribute */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/* Color Attribute*/
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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
		myVector.z = 0.5f;

		/*shader.setFloat("xColor", xValue);
		shader.setFloat("yColor", yValue);
		shader.setFloat("zColor", zValue);*/
		//glUniform3f(glGetUniformLocation(shader.program, "color"), 1.0f, 0.5f, 0.5f);
		//glUniform3fv(glGetUniformLocation(shader.program, "color"),1, glm::value_ptr(myVector));
		//shader.setVec3("color", 1.0f, 0.5f, 0.5f);
		shader.setVec3v("color", myVector);

		// matrix
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, &model[0][0]);
		model = glm::translate(model, glm::vec3(myPos));
		model = glm::rotate(model, glm::radians(45.0f)*time, glm::vec3(xValue, yValue, 0.5f));
		shader.setMat4("model", model);

		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//glUniformMatrix4fv(glGetUniformLocation(shader.program, "model"), 1, GL_FALSE, &model[0][0]);
		model = glm::translate(model, glm::vec3(-0.5f,-0.2f,0.0f));
		model = glm::rotate(model, glm::radians(45.0f) * time, glm::vec3(xValue, yValue, 0.5f));
		shader.setMat4("model", model);
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
		myPos.y += 0.005f;
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_TRUE)
		myPos.y -= 0.005f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_TRUE)
		myPos.x += 0.005f;
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_TRUE)
		myPos.x -= 0.005f;
}

void mouseCursorPosition(GLFWwindow* window, double xPos, double yPos)
{
	std::cout << "mouse moving\n";
}

void mouseScrollPosition(GLFWwindow* window, double xOffset, double yOffset)
{
	std::cout << "mouse scroll\n";
}
