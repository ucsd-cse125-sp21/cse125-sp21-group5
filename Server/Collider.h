#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

class Collider
{
public:
	// The eight corners that make up a box
	//glm::vec3 points[8];
	glm::vec3 center;
	glm::vec3 dim;
	
	// Constructor
	Collider(const glm::vec3& center, const glm::vec3& dimensions);
	
	bool contains(Collider* other);
	bool check_collision(Collider* other);
};
