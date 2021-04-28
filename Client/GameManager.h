#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

// TODO: organize these
#include <iostream>
#include <vector>
#include "Window.h"
#include "Camera.h"
#include "Model.h"
#include "KeyboardInputManager.h"
#include "../Shared/Event.h"
#include "Client.h"

using namespace std;

class GameManager
{
private:
	GLFWwindow* window;
	Camera* camera;
	vector<Model*> models;

	// Not sure how many shaders we'll have
	// but potentially create shader class?
	GLuint shader;

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

	void update(Client& client);

	void render();

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	
};

#endif