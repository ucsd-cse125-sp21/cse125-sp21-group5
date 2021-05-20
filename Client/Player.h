#pragma once

#include <string>
#include "Camera.h"
#include "Model.h"
#include "Transform.h"
#include "../Shared/GameState.h"

class Player : public Node
{
public:
	Camera* cam;
	Transform* transform;

	Model* model;
	std::vector<Model*> models;
	float health;
	int playerId;

private:
	bool mustLoadModels;

public:
	Player(Transform* transform, int playerId);
	~Player();

	void draw(const glm::mat4& parent_transform, const glm::mat4& view);
	void update(float deltaTime);

	void loadModels();
	// Update Camera attributes
	void updatePlayer(PlayerState ps);

	void displayHUD();
};

