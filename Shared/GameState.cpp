#include "GameState.h"

GameState::GameState(glm::vec3 pos, glm::vec3 front) {
	//this->posX = pos.x;
	//this->posY = pos.y;
	//this->posZ = pos.z;
	this->pos = pos;
	this->front = front;
}

GameState::GameState() {
	//this->posX = 0;
	//this->posY = 0;
	//this->posZ = 0;
	this->pos = glm::vec3(0);
	this->front = glm::vec3(0);
}