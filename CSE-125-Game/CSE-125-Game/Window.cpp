#include "Window.h"

#include <string>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GLU.h>
#include <GL/GL.h>

#include <glm/glm.hpp>
using namespace glm;

float deltaTime = 0.0f;
float lastFrame = glfwGetTime();

bool w, s, a, d = false;
glm::vec3 dir = glm::vec3(0.0f);

GLFWwindow* Window::create(int width, int height, std::string title) {

	if (!glfwInit()) {
		std::cerr << "Failed to initalize GLFW" << std::endl;
		return NULL;
	}

	// set up GLFW window values
	glfwWindowHint(GLFW_SAMPLES, 4);					// set 4x MSAA
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);		// set GL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use new opengl features

	GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

	if (window == NULL) {
		std::cerr << "Failed to open GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);


	// set up GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to init GLEW" << std::endl;
	}

	glEnable(GL_DEPTH_TEST);

	return window;
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
			// Controls to move player
		case GLFW_KEY_W:
			printf("W");
			w = true;
			break;
		case GLFW_KEY_S:
			printf("S");
			s = true;
			break;
		case GLFW_KEY_A:
			printf("A");
			a = true;
			break;
		case GLFW_KEY_D:
			printf("D");
			d = true;
			break;
		default:
			break;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		switch (key)
		{
			// Stop camera movement
		case GLFW_KEY_W:
			printf("W");
			w = false;
			break;
		case GLFW_KEY_S:
			printf("S");
			s = false;
			break;
		case GLFW_KEY_A:
			printf("A");
			a = false;
			break;
		case GLFW_KEY_D:
			printf("D");
			d = false;
			break;
		default:
			break;
		}
	}
}

void Window::idleCallback()
{
	/* Movement */
	// up
	if (w)
	{
		printf("w");
		dir = glm::vec3(0.0f, 1.0f, 0.0f);
	}
	// down
	else if (s)
	{
		printf("s");
		dir = glm::vec3(0.0f, -1.0f, 0.0f);
	}
	// left
	if (a)
	{
		printf("a");
		dir = glm::vec3(-1.0f, 0.0f, 0.0f);
	}
	// right
	else if (d)
	{
		printf("d");
		dir = glm::vec3(1.0f, 0.0f, 0.0f);
	}
}

void Window::displayCallback(GLFWwindow* window)
{
	// Consistent movement
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
}

void Window::swapBuffers(GLFWwindow* window) {
	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool Window::shouldClose(GLFWwindow* window) {
	return glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(window);
}
