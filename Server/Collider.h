#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

class Collider
{
public:
	// Define box using center point and dimensions
	glm::vec3 center;
	glm::vec3 dim;
	
	// Constructor
	Collider(const glm::vec3& center, const glm::vec3& dimensions);
	
	bool contains(Collider* other);
	glm::vec3 check_collision(Collider* other);
};
