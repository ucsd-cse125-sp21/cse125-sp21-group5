#include "ServerPlayer.h"

const glm::vec3 PLAYER_COLLIDER_OFFSET = glm::vec3(0, -0.38, 0);

ServerPlayer::ServerPlayer(const glm::vec3& initPos,
						   float initYaw,
						   float initPitch,
						   int playerId)
{
	pos = glm::vec3(0, 0, 0);
	yaw = 0;
	pitch = 0;

	hitbox = new Collider(ObjectType::PLAYER, pos + PLAYER_COLLIDER_OFFSET, glm::vec3(1, 2.2f, 1));
	hitbox->setParentPlayer(this);

	this->update(initPos, initYaw, initPitch);

	vVelocity = -0.1f;
	animation = AnimationID::IDLE;
	jumping = 0;
	isGrounded = false;
	health = 100.0f;
	isDead = 0;
	kills = 0;
	deaths = 0;
	captures = 0;

	// TODO: why do we need to push guns?
	gun_idx = 0;
	guns.push_back(new Pistol());
	guns.push_back(new FOV());

	isShooting = false;
	isLimitFOV = 0;
	isFogged = 0;
	isFrozen = 0;
	playerClass = 0;

	this->team =
		(playerId % 2 == (int)PlayerTeam::CAT_LOVER)
		? PlayerTeam::CAT_LOVER
		: PlayerTeam::DOG_LOVER;
}

void ServerPlayer::update(const glm::vec3& dPos, const float dYaw, const float dPitch)
{
	// Calculate new pitch and yaw
	yaw += dYaw;
	pitch += dPitch;
	pitch = glm::clamp(pitch, -89.0f, 89.0f);

	// Calculate new front
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);


	// Update position of camera and collider
	pos += dPos;
	hitbox->set_center(pos + PLAYER_COLLIDER_OFFSET);
}


void ServerPlayer::updateAnimations(const Event& e) {
	// choose the animation to play on the client
	if (guns[gun_idx]->reload_time > 0) {
		animation = AnimationID::RELOAD;
	}
	else if (e.shooting)
	{
		animation = AnimationID::SHOOT;
	}
	else if (glm::length(e.dPos) > 0) {
		// the player is moving, play the run animation
		animation = AnimationID::WALK;
	}
	else {
		animation = AnimationID::IDLE;
	}
}

bool ServerPlayer::isDeadCheck() {
	return (this->health <= 0.0f);
}

void ServerPlayer::decreaseHealth(float decAmount)
{
	health -= decAmount;
}

void ServerPlayer::resetPlayer(glm::vec3 pos, float yaw, float pitch)
{
	respawn(pos, yaw, pitch);

	// Reset Player scores
	kills = 0;
	captures = 0;
	deaths = 0;
	playerClass = 0;
	isReady = false;
}

void ServerPlayer::respawn(glm::vec3 pos, float yaw, float pitch)
{
	// Reset camera and position variables
	this->pos = glm::vec3(0.0f);
	this->yaw = 0;
	this->pitch = 0;

	// Reset and lingering special effects
	isShooting = false;
	isLimitFOV = 0;
	isFogged = 0;
	isFrozen = 0;

	// Reset player properties
	vVelocity = -0.1f;
	animation = AnimationID::IDLE;
	jumping = 0;
	isGrounded = false;
	health = 100.0f;
	isDead = 0;
	hitbox->isActive = true;

	// TODO: reset gun variables

	// Move camera
	update(pos, yaw, pitch);
}