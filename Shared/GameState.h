#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <boost/serialization/vector.hpp>

enum class AnimationID {
	IDLE = 0,
	WALK = 1,
	SHOOT = 2
};

class PlayerState
{
public:
	int playerId;
	glm::vec3 pos, front;
	float health;

	bool isGrounded;
	int isAlive;
	AnimationID currentAnimation;

	bool carryingFlag;

	PlayerState(int playerId, 
				const glm::vec3& pos,
				const glm::vec3& front,
				AnimationID currentAnimation,
				bool isGrounded,
				float health,
				int isAlive,
				bool carryingFlag);
	PlayerState();

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
		ar& isAlive;
		ar& carryingFlag;
	}
};

class GameState
{
public:
	std::vector<PlayerState> states;

	void addState(PlayerState ps);

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& states;
	}

};