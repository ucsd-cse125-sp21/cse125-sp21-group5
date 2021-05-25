#pragma once

#include <GL/glew.h>
#include "glm/glm.hpp"
#include <iostream>
#include "Collider.h"
#include "../Shared/GameState.h"
#include "../Shared/Event.h"
#include "../Shared/Global_variables.h"

class ServerPlayer
{
public:
	// Camera variables
	glm::vec3 front;
	glm::vec3 pos;
	float yaw, pitch;
	float health;
	float vVelocity;

	//Team
	PlayerTeam team;

	AnimationID animation;

	// Collision/movement/jumping variables
	Collider* hitbox; 
	bool isGrounded;
	int jumping;
	int isDead;

	ServerPlayer();
	ServerPlayer(const glm::vec3& initPos, int playerId);
	ServerPlayer(const glm::vec3& initPos,
		   const glm::vec3& hitboxSize,
		   float initYaw,
		   float initPitch);

	void update(const glm::vec3& dPos, const float dYaw, const float dPitch);
	void decreaseHealth(float decAmount);
	bool isDeadCheck();
	void respawn();
	void updateAnimations(const Event& e);
};

