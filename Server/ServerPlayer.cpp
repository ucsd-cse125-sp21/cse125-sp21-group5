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
}

ServerPlayer::ServerPlayer(const glm::vec3& initPos) {
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
}

ServerPlayer::ServerPlayer(const glm::vec3& initPos,
			   const glm::vec3& hitboxSize,
			   float initYaw,
			   float initPitch)
{
	hitbox = new Collider(ObjectType::PLAYER, initPos, hitboxSize);
	hitbox->setParentPlayer(this);
	update(initPos, initYaw, initPitch);
	health = 100.0f;
	vVelocity = -0.1f;
	animation = AnimationID::IDLE;
	isDead = 0;
}

void ServerPlayer::update(const glm::vec3& dPos, const float dYaw, const float dPitch) {
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
	if (glm::length(e.dPos) > 0) {
		// the player is moving, play the run animation
		animation = AnimationID::WALK;
	}
	else {
		animation = AnimationID::IDLE;
	}
}

bool ServerPlayer::isDeadCheck() {
	return (this->health <= 0.5f);
}

void ServerPlayer::decreaseHealth(float decAmount)
{
	health -= decAmount;
	if (isDeadCheck())
	{
		this->isDead = DEATH_TICK_TIMER;
		hitbox->isActive = false;
	}
}