#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

// TODO: organize these
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <vector>

#include "Tile.h"
#include "Window.h"
#include "Camera.h"
#include "../Shared/Event.h"
#include "../Shared/MapState.h"
#include "Model.h"
#include "Transform.h"

using namespace std;

class GameManager
{
private:
	// Important variables
	GLFWwindow* window;

	//Client client = Client();

	// Root of scene graph
	
	Transform* monkeT;


	// Calculate deltaTime to ensure consistent movement
	float deltaTime, prevTime, currTime;

	// Mouse variables
	static float offsetX, offsetY, lastX, lastY;
	static float fov;

public:
	// Important variables
	Camera* camera;

	// TODO: not supposed to be public scene graph vars
	Transform* worldT;
	Transform* playerT;
	Transform* cubeT1;
	Transform* cubeT2;
	Transform* cubeT3;

	GameManager(GLFWwindow * window);
	~GameManager();

	Event update();

	void render();

	Event handleInput();
	
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	void updateMap(MapState& map);

};


#endif