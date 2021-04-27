#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

class Collider
{
public:
	glm::vec3 center;
	float radius;

	enum class Type
	{
		sphere,
		box,
		plane
	};

	Collider::Type type;

	Collider(const glm::vec3& center, const float& radius);
	Collider(const glm::vec3& dimensions);

	bool check_collision(Collider* other);
};