#pragma once

#include <GL/glew.h>
#include "glm/glm.hpp"
#include <iostream>
#include "Collider.h"

class Player
{
public:
	glm::vec3 pos;
	glm::vec3 front;
	float yaw, pitch;

	Collider* hitbox;

	Player();
	Player(glm::vec3 initPos);
	void update(glm::vec3 pos, float yaw, float pitch);
};

