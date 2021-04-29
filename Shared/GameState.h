#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class GameState
{
public:
	float posX, posY, posZ;

	GameState(glm::vec3 pos);
	GameState();

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& posX;
		ar& posY;
		ar& posZ;
	}
};

