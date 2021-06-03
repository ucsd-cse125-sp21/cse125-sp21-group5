#pragma once
#include <vector>
#include <limits>
#include <unordered_map>

#include "ServerPlayer.h"
#include "../Shared/Event.h"
#include "../Shared/GameState.h"
#include "../Shared/MapState.h"
#include "../Shared/Global_variables.h"
#include "Quadtree.h"


using namespace std;

class ServerGameManager
{
public:
	// To keep track of all the particles/hitboxes 
	// TODO: need to be able to make this generic 
	vector<Collider*> allColliders;

	// Quadtree variable 
	Quadtree* qt;
	Collider* tileC;
	Collider* flagCat, *flagDog;
	Collider* catWinArea, *dogWinArea;
	int flagCatCarrierId, flagDogCarrierId;

	// Indicates game status
	bool gameStarted = false;
	int gameCountdown = -1;
	unsigned int gameOverCountdown = GAMEOVER_TIMER;

	State gameStatus;

	// Win condition bools 
	bool dogTeamWin = false;
	bool catTeamWin = false;
	PlayerTeam winningTeam = PlayerTeam::NOBODY;

	// Track all players
	unordered_map<int, ServerPlayer*> players;

	ServerGameManager();

	void createNewPlayer(int playerId);

	void handleEvent(Event& e, int playerId);
	void handleDeath(ServerPlayer* player);
	void handleShoot(ServerPlayer* player);
	void handleMovement(ServerPlayer* player, int playerId, Event& e);

	GameState getGameState(int playerId);
	MapState generateMap();
	void buildQuadtree();
	bool checkWinCondition();
	void forceDab(PlayerTeam team);
	void switchClass(int playerId, int playerClass);
	void resetPlayersToSpawn();
	void respawnPlayerWithID(int playerId, glm::vec3 pos, float yaw, float pitch);
	void gameOver();
	

	int tileSeed;
};

