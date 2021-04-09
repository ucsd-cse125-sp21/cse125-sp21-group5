#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Model.h"


class Window
{
private:

public:
	//static Model* monke;
	static GLFWwindow* create(int width, int height, std::string title);
	static void swapBuffers(GLFWwindow* window);
	static bool shouldClose(GLFWwindow* window);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void idleCallback();
	static void displayCallback(GLFWwindow* window);
};

