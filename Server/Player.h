#pragma once

#include <GL/glew.h>
#include "glm/glm.hpp"
#include <iostream>
#include "Collider.h"

class Player
{
public:
	// Camera variables
	glm::vec3 front;
	glm::vec3 pos;
	float yaw, pitch;

	// Collision variables
	Collider* hitbox; 

	Player();
	Player(const glm::vec3& initPos);
	Player(const glm::vec3& initPos,
		   const glm::vec3& hitboxSize,
		   float initYaw,
		   float initPitch);

	void update(const glm::vec3& dPos, const float dYaw, const float dPitch);
};

