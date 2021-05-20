#pragma once

#include <GL/glew.h>
#include "glm/glm.hpp"
#include <iostream>
#include "Collider.h"
#include "../Shared/GameState.h"

class ServerPlayer
{
public:
	// Camera variables
	glm::vec3 front;
	glm::vec3 pos;
	float yaw, pitch;

	AnimationID animation;

	// Collision variables
	Collider* hitbox; 

	ServerPlayer();
	ServerPlayer(const glm::vec3& initPos);
	ServerPlayer(const glm::vec3& initPos,
		   const glm::vec3& hitboxSize,
		   float initYaw,
		   float initPitch);

	void update(const glm::vec3& dPos, const float dYaw, const float dPitch);
};

