#include "Player.h"

Player::Player() {}

void Player::update(glm::vec3 pos, float yaw, float pitch) {
	// Calculate new pitch and yaw
	this->yaw += yaw;
	this->pitch += pitch;
	this->pitch = glm::clamp(this->pitch, -89.0f, 89.0f);


	// Calculate new front
	front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	front.y = sin(glm::radians(this->pitch));
	front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	front = glm::normalize(this->front);

	this->pos = pos;
}