#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

class Collider
{
public:
	// Define box using center point and dimensions
	glm::vec3 cen;
	glm::vec3 dim;
	glm::vec3 min;
	glm::vec3 max;
	
	// Constructor
	Collider(const glm::vec3& center, const glm::vec3& dimensions);
	
	bool contains(Collider* other);
	glm::vec3 check_collision(Collider* other);
	bool check_ray_collision(glm::vec3 origin, glm::vec3 dir, glm::vec3& hitPosition);
	bool check_point_collision(glm::vec3 point);
};
