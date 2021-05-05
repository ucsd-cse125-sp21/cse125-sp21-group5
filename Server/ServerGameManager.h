#pragma once
#include <vector>
#include "Player.h"
#include "../Shared/Event.h"
#include "../Shared/GameState.h"

using namespace std;

class ServerGameManager
{
public:
	vector<Player> players;
	ServerGameManager();
	void handleEvent(Event& e, int playerId);
	GameState getGameState(int playerId);
};

