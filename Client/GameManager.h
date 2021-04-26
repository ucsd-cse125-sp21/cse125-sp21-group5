#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

// TODO: organize these
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "Window.h"
#include "Camera.h"
#include "Model.h"
#include "Transform.h"

using namespace std;

class GameManager
{
private:
	// Objects in world
	GLFWwindow* window;
	Camera* camera;

	// Root of scene graph
	Transform* worldT;
	Transform* playerT;

	// Calculate deltaTime to ensure consistent movement
	float deltaTime, prevTime, currTime;

	// TODO: overkill?
	// Detect all keyboard pressed
	//static int keyStateMap[];
	static bool W, S, A, D, SPACE, L_CTRL;

	// Mouse variables
	static float offsetX, offsetY, lastX, lastY;
	static float fov;

public:
	GameManager(GLFWwindow * window);
	~GameManager();

	void update();

	void render();

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};

#endif