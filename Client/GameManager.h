#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

// TODO: organize these
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "Tile.h"
#include "Window.h"
#include "../Shared/Camera.h"
#include "Model.h"
#include "Transform.h"
#include "Client.h"

using namespace std;

class GameManager
{
private:
	// Important variables
	GLFWwindow* window;
	//Client client = Client();

	// Root of scene graph
	Transform* worldT;
	Transform* playerT;
	Transform* monkeT;

	// Calculate deltaTime to ensure consistent movement
	float deltaTime, prevTime, currTime;

	// Mouse variables
	static float offsetX, offsetY, lastX, lastY;
	static float fov;

public:
	Camera* camera;

	GameManager(GLFWwindow * window);
	~GameManager();

	void update(Client& client);

	void render();

	void handleInput(Client& client);

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};

#endif