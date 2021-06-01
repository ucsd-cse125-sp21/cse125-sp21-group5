#include "GameState.h"
#include <iostream>

PlayerState::PlayerState()
{
	this->playerId = 0;
	this->pos = glm::vec3(0.0f);
	this->front = glm::vec3(0.0f);
	this->isGrounded = false;
	this->health = -1.0f;
	this->currentAnimation = AnimationID::IDLE;
	this->isDead = 1;
	this->carryingCatFlag = false;
	this->carryingDogFlag = false;
	this->kills = 0;
	this->deaths = 0;
	this->captures = 0;
	this->hasFiredGun = false;
}

// TODO: why is this not a shared player?
PlayerState::PlayerState(int playerId, 
						const glm::vec3& pos,
						const glm::vec3& front,
						AnimationID currentAnimation,
						bool isGrounded,
						float health,
						int isAlive,
						bool carryingCatFlag,
						bool carryingDogFlag,
						unsigned int kills,
						unsigned int deaths,
						unsigned int captures,
						int gun_idx,
						const Gun& curr_gun,
						bool isShooting,
						int isLimitFOV,
						int isFogged,
						int isFrozen,
						int playerClass,
						bool hasFiredGun)
{
	this->playerId = playerId;
	this->pos = pos;
	this->front = front;
	this->currentAnimation = currentAnimation;
	this->isGrounded = isGrounded;
	this->health = health;
	this->isDead = isAlive;
	this->carryingCatFlag = carryingCatFlag;
	this->carryingDogFlag = carryingDogFlag;
	this->kills = kills;
	this->deaths = deaths;
	this->captures = captures;
	this->gun_idx = gun_idx;
	this->curr_gun = curr_gun;
	this->isShooting = isShooting;
	this->isLimitFOV = isLimitFOV;
	this->isFogged = isFogged;
	this->isFrozen = isFrozen;
	this->playerClass = playerClass;
	this->hasFiredGun = hasFiredGun;
}

// Could be reference? 
void GameState::addState(PlayerState ps)
{
	states.push_back(ps);
}