#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

// TODO: organize these
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <vector>
#include <unordered_map>

#include "Window.h"
#include "Camera.h"
#include "../Shared/Event.h"
#include "../Shared/MapState.h"
#include "Model.h"
#include "Transform.h"
#include "Player.h"

using namespace std;

class GameManager
{
private:
	// Window for callback functions
	GLFWwindow* window;

	// Calculate deltaTime to ensure consistent movement
	float deltaTime, prevTime, currTime;

	// Mouse variables
	static float offsetX, offsetY, lastX, lastY;
	static float fov;

	// Keyboard variables
	bool showScoreboard;

	// Global bool for win condition
	bool catTeamWin;
	bool dogTeamWin;

public:
	// Track players in the world
	unordered_map<int, Player*> players;
	int localPlayerId;

	// Models used in the environment
	Model* playerModel;
	Model* tileModel;
	vector<Model*> treeModels;

	Transform* catT;
	Model* catModel;
	Transform* dogT;
	Model* dogModel;

	// TODO: not supposed to be public scene graph vars
	Transform* worldT;

	GameManager(GLFWwindow * window);
	~GameManager();

	void render();
	void renderUI();

	Event handleInput();
	Event update();
	
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	void updateMap(MapState& map);

	void addPlayer(int playerId, Model* transform);
	void updateGameState(GameState& gs);

	void setLocalPlayerID(int playerId);
};


#endif