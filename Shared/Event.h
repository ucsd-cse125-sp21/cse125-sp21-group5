#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Event
{
public:
	glm::vec3 dir;
	float speed;
	glm::vec3 position;

	Event(glm::vec3 dir, float speed, glm::vec3 position);
};