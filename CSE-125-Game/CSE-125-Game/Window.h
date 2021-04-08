#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


class Window
{
private:

public:
	static GLFWwindow* create(int width, int height, std::string title);
	static void swapBuffers(GLFWwindow* window);
	static bool shouldClose(GLFWwindow* window);
};

