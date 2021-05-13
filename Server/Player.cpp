#include "Player.h"

Player::Player() {
	this->pos = glm::vec3(0.0f);
	this->front = glm::vec3(0.0f, 0.0f, 1.0f);
	this->yaw = 0.0f;
	this->pitch = 0.0f;
	this->hitbox = new Collider(this->pos, glm::vec3(1.0f));
}

Player::Player(glm::vec3 initPos) {
	this->pos = initPos;
	this->front = glm::vec3(0.0f, 0.0f, 1.0f);
	this->yaw = 0.0f;
	this->pitch = 0.0f;
	this->hitbox = new Collider(this->pos, glm::vec3(1.0f));
}


// TODO: pos is supposed to be deltaPos
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

	this->pos += pos;

	// Move the collider
	this->hitbox->center = this->pos;
}