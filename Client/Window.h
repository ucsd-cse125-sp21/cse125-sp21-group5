#ifndef _WINDOW_H_
#define _WINDOW_H_
#include "main.h"
#include "Model.h"
#include "Shader.h"
#include "Camera.h"

#include "Cube.h"
#include <string>

using namespace std;

class Window
{
private:

public:
	// Window properties
	static int width;
	static int height;
	static const char* windowTitle;

	// Camera variables (TODO: maybe move)
	static glm::mat4 projection;

	// Constructor
	static GLFWwindow* create(int width, int height, string title);

	// Destructors
	static void cleanUp();

	// Handle resize
	static void resizeCallback(GLFWwindow* window, int width, int height);
};

#endif