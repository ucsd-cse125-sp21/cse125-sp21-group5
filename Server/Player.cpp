#include "Player.h"

Player::Player() {
	pos = glm::vec3(0.0f);
	front = glm::vec3(0.0f, 0.0f, 1.0f);
	yaw = 0.0f;
	pitch = 0.0f;
	hitbox = new Collider(pos, glm::vec3(1.0f));
}

Player::Player(const glm::vec3& initPos) {
	pos = initPos;
	front = glm::vec3(0.0f, 0.0f, 1.0f);
	yaw = 0.0f;
	pitch = 0.0f;
	hitbox = new Collider(pos, glm::vec3(1.0f));
}

Player::Player(const glm::vec3& initPos,
			   const glm::vec3& hitboxSize,
			   float initYaw,
			   float initPitch)
{
	hitbox = new Collider(initPos, hitboxSize);
	update(initPos, initYaw, initPitch);
}

void Player::update(const glm::vec3& dPos, const float dYaw, const float dPitch) {
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
	hitbox->center = pos;
}