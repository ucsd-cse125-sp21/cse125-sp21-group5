#pragma once

#include <GL/glew.h>
#include "glm/glm.hpp"
#include <iostream>

class Player
{
public:
	glm::vec3 pos;
	glm::vec3 front;
	float yaw, pitch;

	Player();
	void update(glm::vec3 pos, float yaw, float pitch);
};

