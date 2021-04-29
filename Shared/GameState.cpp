#include "GameState.h"

GameState::GameState(glm::vec3 pos) {
	this->posX = pos.x;
	this->posY = pos.y;
	this->posZ = pos.z;
}

GameState::GameState() {
	this->posX = 0;
	this->posY = 0;
	this->posZ = 0;
}