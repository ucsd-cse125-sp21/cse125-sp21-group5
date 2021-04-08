#include "Window.h"

#include <string>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GLU.h>
#include <GL/GL.h>

#include <glm/glm.hpp>
using namespace glm;

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

void Window::swapBuffers(GLFWwindow* window) {
	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool Window::shouldClose(GLFWwindow* window) {
	return glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(window);
}
