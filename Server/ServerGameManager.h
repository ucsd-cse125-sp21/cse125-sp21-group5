#pragma once
#include <vector>
#include <limits>
#include <unordered_map>

#include "ServerPlayer.h"
#include "../Shared/Event.h"
#include "../Shared/GameState.h"
#include "../Shared/MapState.h"
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

	bool gameStarted = false;
	int gameCountDown = 0;

	// Track all players
	unordered_map<int, ServerPlayer*> players;

	ServerGameManager();

	void createNewPlayer(int playerId);

	void handleEvent(Event& e, int playerId);
	void handleShoot(ServerPlayer* player);

	GameState getGameState(int playerId);
	MapState generateMap();
	void buildQuadtree();

	

	int tileSeed;
};

