#include "ServerGameManager.h"

ServerGameManager::ServerGameManager() {
	players.push_back(Player());
}

void ServerGameManager::handleEvent(Event& e, int playerId) {
	players[playerId].update(e.pos, e.yaw, e.pitch);
}

GameState ServerGameManager::getGameState(int playerId) {
	return GameState(players[playerId].pos, players[playerId].front);
}
