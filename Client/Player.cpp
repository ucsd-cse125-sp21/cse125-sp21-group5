#include "Player.h"

Player::Player(Transform* transform, Model* model) :
	transform(transform), model(model)
{
	cam = new Camera();
}

void Player::draw(const glm::mat4& parent_transform, const glm::mat4& view) {
	model->draw(parent_transform, view);
}

void Player::update(float deltaTime)
{
	model->update(deltaTime);
}

void Player::updatePlayer(PlayerState ps) {
	cam->update(ps.pos, ps.front);
	transform->setTranslate(ps.pos + glm::vec3(0.0f, -2.0f, 0.0f));
}