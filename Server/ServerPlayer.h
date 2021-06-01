#pragma once

#include <GL/glew.h>
#include "glm/glm.hpp"
#include <iostream>

#include "Collider.h"
#include "../Shared/GameState.h"
#include "../Shared/Event.h"
#include "../Shared/Global_variables.h"
#include "../Shared/Gun.h"

class ServerPlayer
{
public:
	// Camera variables
	glm::vec3 front;
	glm::vec3 pos;
	float yaw, pitch;

	float health;
	float vVelocity;

	// Team
	PlayerTeam team;

	AnimationID animation;

	// Score variables
	unsigned int kills, deaths, captures;

	// Collision/movement/jumping variables
	Collider* hitbox; 
	bool isGrounded;
	int jumping;
	int isDead;

	// Guns
	int gun_idx;
	std::vector<Gun*> guns;

	int isLimitFOV;
	int isFogged;
	int isFrozen;
	int playerClass;

	bool hasFiredGun;

	ServerPlayer();
	ServerPlayer(const glm::vec3& initPos, int playerId);
	ServerPlayer(const glm::vec3& initPos,
			    float initYaw,
			    float initPitch,
				int playerId);

	void update(const glm::vec3& dPos, const float dYaw, const float dPitch);
	void decreaseHealth(float decAmount);
	bool isDeadCheck();
	void resetPlayer(glm::vec3 pos, float yaw, float pitch);
	void respawn(glm::vec3 pos, float yaw, float pitch);
	void updateAnimations(const Event& e);
};

