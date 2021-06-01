#include "ServerPlayer.h"
#include <glm/gtx/string_cast.hpp>

const glm::vec3 PLAYER_COLLIDER_OFFSET = glm::vec3(0, -0.38, 0);

ServerPlayer::ServerPlayer() {
	pos = glm::vec3(0.0f);
	front = glm::vec3(0.0f, 0.0f, 1.0f);
	yaw = 0.0f;
	pitch = 0.0f;
	vVelocity = -0.1f;
	hitbox = new Collider(ObjectType::PLAYER, pos, glm::vec3(1.0f));
	hitbox->setParentPlayer(this);
	animation = AnimationID::IDLE;
	isGrounded = false;
	jumping = 0;
	health = 100.0f;
	isDead = 0;
	kills = 0;
	deaths = 0;
	captures = 0;

	isShooting = false;
	isLimitFOV = 0;
	isFogged = 0;
	isFrozen = 0;
	playerClass = 0;
	hasFiredGun = false;
}

ServerPlayer::ServerPlayer(const glm::vec3& initPos, int playerId)
{
	pos = initPos;
	front = glm::vec3(0.0f, 0.0f, 1.0f);
	yaw = 0.0f;
	pitch = 0.0f;
	hitbox = new Collider(ObjectType::PLAYER, pos + PLAYER_COLLIDER_OFFSET, glm::vec3(1, 2.2f, 1));
	hitbox->setParentPlayer(this);
	vVelocity = -0.1f;
	animation = AnimationID::IDLE;
	jumping = 0;
	isGrounded = false;
	health = 100.0f;
	isDead = 0;
	kills = 0;
	deaths = 0;
	captures = 0;

	gun_idx = 0;
	guns.push_back(new Pistol());
	guns.push_back(new FOV());

	isShooting = false;
	isLimitFOV = 0;
	isFogged = 0;
	isFrozen = 0;
	playerClass = 0;

	hasFiredGun = false;

	this->team =
		(playerId % 2 == (int) PlayerTeam::CAT_LOVER)
			? PlayerTeam::CAT_LOVER
			: PlayerTeam::DOG_LOVER;
}

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

	gun_idx = 0;
	guns.push_back(new Pistol());
	guns.push_back(new FOV());

	isShooting = false;
	isLimitFOV = 0;
	isFogged = 0;
	isFrozen = 0;
	playerClass = 0;

	hasFiredGun = false;

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
	if (e.shooting)
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

	kills = 0;
	captures = 0;
	deaths = 0;
	vVelocity = -0.1f;
	animation = AnimationID::IDLE;
	jumping = 0;
	isGrounded = false;
	health = 100.0f;
	isDead = 0;

	isShooting = false;
	isLimitFOV = 0;
	isFogged = 0;
	isFrozen = 0;
	playerClass = 0;
}

void ServerPlayer::respawn(glm::vec3 pos, float yaw, float pitch)
{
	this->pos = glm::vec3(0, 0, 0);
	this->yaw = 0;
	this->pitch = 0;

	update(pos, yaw, pitch);
}