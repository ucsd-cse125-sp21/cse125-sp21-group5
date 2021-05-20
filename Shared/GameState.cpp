#include "GameState.h"

PlayerState::PlayerState() {
	this->playerId = 0;
	this->pos = glm::vec3(0.0f);
	this->front = glm::vec3(0.0f);

	this->currentAnimation = AnimationID::IDLE;
}

PlayerState::PlayerState(int playerId, const glm::vec3& pos, const glm::vec3& front, AnimationID currentAnimation, bool isColliding) {
	this->playerId = playerId;
	this->pos = pos;
	this->front = front;
	this->isColliding = isColliding;
	this->currentAnimation = currentAnimation;
}

void GameState::addState(PlayerState& ps)
{
	states.push_back(ps);
}