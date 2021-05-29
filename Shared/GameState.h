#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>

#include "../Shared/Gun.h"

enum class AnimationID {
	IDLE = 0,
	WALK = 1,
	SHOOT = 2,
	DAB = 3,
	DEATH = 4,
	RELOAD = 5
};

class PlayerState
{
public:
	int playerId;
	glm::vec3 pos, front;

	AnimationID currentAnimation;
	bool isGrounded;
	float health;
	int isDead;
	
	bool carryingCatFlag;
	bool carryingDogFlag;	

	unsigned int kills, deaths, captures;

	int gun_idx;
	Gun curr_gun;

	int isLimitFOV, isFogged, isFrozen;
	int playerClass;

	PlayerState();
	PlayerState(int playerId, 
				const glm::vec3& pos,
				const glm::vec3& front,
				AnimationID currentAnimation,
				bool isGrounded,
				float health,
				int isAlive,
				bool carryingCatFlag,
				bool carryingDogFlag,
				unsigned int kills,
				unsigned int deaths,
				unsigned int captures,
				int gun_idx,
				const Gun& curr_gun,
				int isLimitFOV,
				int isFogged,
				int isFrozen,
				int playerClass
	);
	

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& playerId;
		ar& pos.x;
		ar& pos.y;
		ar& pos.z;
		ar& front.x;
		ar& front.y;
		ar& front.z;
		ar& currentAnimation;
		ar& isGrounded;
		ar& health;
		ar& isDead;
		ar& carryingCatFlag;
		ar& carryingDogFlag;
		ar& kills;
		ar& deaths;
		ar& captures;
		ar& gun_idx;
		ar& curr_gun;
		ar& isLimitFOV;
		ar& isFogged;
		ar& isFrozen;
		ar& playerClass;
	}
};

class GameState
{
public:
	std::vector<PlayerState> states;
	glm::vec3 catLocation;
	glm::vec3 dogLocation;

	bool catTeamWin;
	bool dogTeamWin;

	int gameCountdown;

	void addState(PlayerState ps);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& states;
		ar& catLocation.x;
		ar& catLocation.y;
		ar& catLocation.z;
		ar& dogLocation.x;
		ar& dogLocation.y;
		ar& dogLocation.z;
		ar& catTeamWin;
		ar& dogTeamWin;
		ar& gameCountdown;
	}

};