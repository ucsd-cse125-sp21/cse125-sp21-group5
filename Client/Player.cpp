#include "Player.h"

// TODO: Used for yourself?
/*
Player::Player(Transform* transform)
{
	this->transform = transform;
	this->model = nullptr;
	cam = new Camera();
}
*/

// Used for other players
Player::Player(Transform* transform, Model* model)
{
	this->transform = transform;
	this->model = model;
	cam = new Camera();
}

void Player::draw(const glm::mat4& parent_transform, const glm::mat4& view)
{
	// don't draw null model anything if model is null
	if (model == nullptr) return;
	model->draw(parent_transform, view);
}

void Player::update(float deltaTime)
{
	if (model == nullptr) return;
	model->update(deltaTime);
}

void Player::updatePlayer(PlayerState ps) {
	cam->update(ps.pos, ps.front);
	transform->setTranslate(ps.pos);
	// TODO: transform->setRotate(ps.front);
}