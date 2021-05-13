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

public:
	Player(Transform* transform, Model* model);

	void draw(const glm::mat4& parent_transform, const glm::mat4& view);
	void update(float deltaTime);

	// Update Camera attributes
	void updatePlayer(PlayerState ps);
	
};

