#pragma once
#include <vector>
#include "Player.h"
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
	BoxCollider worldBox = BoxCollider(glm::vec3(0.0f, 0.0f, 0.0f),
									   glm::vec3(200.0f, 200.0f, 200.0f));
	vector<BoxCollider*> allColliders;

	// Quadtree variable 
	Quadtree* qt;

	vector<Player> players;
	ServerGameManager();
	void handleEvent(Event& e, int playerId);
	void buildQuadtree();
	GameState getGameState(int playerId);
	MapState generateMap();
};

