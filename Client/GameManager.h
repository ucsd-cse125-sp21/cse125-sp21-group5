#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <boost/format.hpp>

#include "../Shared/Event.h"
#include "../Shared/MapState.h"
#include "../Shared/Global_variables.h"

#include "Window.h"
#include "Camera.h"
#include "Model.h"
#include "Transform.h"
#include "Player.h"
#include "Renderer.h"
#include "AudioManager.h"

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
	PlayerTeam winningTeam;

	// Game status indicator counter
	int gameCountdown;

public:
	// Track players in the world
	unordered_map<int, Player*> players;
	int localPlayerId;

	// For disrespecting, god damnit shane
	bool isDisrespecting;

	// Models used in the environment
	Model* playerModel;
	Model* tileModel;
	Model* wallTileModel;
	vector<Model*> treeModels;

	Transform* catT;
	Model* catModel;
	Transform* dogT;
	Model* dogModel;
	Model* dogTeamIndicator;
	Model* catTeamIndicator;

	bool isReady;

	Transform* catSpawnPlatform;
	Transform* dogSpawnPlatform;
	Model* spawnPlatform;

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