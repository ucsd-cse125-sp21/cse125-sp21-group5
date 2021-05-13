#include "GameState.h"

PlayerState::PlayerState(int playerId, glm::vec3 pos, glm::vec3 front) {
	//this->posX = pos.x;
	//this->posY = pos.y;
	//this->posZ = pos.z;
	this->playerId = playerId;
	this->pos = pos;
	this->front = front;
}

PlayerState::PlayerState() {
	//this->posX = 0;
	//this->posY = 0;
	//this->posZ = 0;
	this->playerId = 0;
	this->pos = glm::vec3(0);
	this->front = glm::vec3(0);
}

GameState::GameState() { }

void GameState::addState(PlayerState ps)
{
	states.push_back(ps);
}