#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

// Forward declare class
class ServerPlayer;

enum class ObjectType
{
	ENVIRONMENT = 0,
	PLAYER = 1,
	GROUND = 2,
	FLAG_CAT = 3,
	FLAG_DOG = 4,
};

class Collider
{
public:
	// Define box using center point and dimensions
	glm::vec3 cen;
	glm::vec3 dim;
	glm::vec3 min;
	glm::vec3 max;

	ObjectType type;
	ServerPlayer* parentPlayerObject;
	
	// Constructor
	Collider() {}
	Collider(ObjectType type, const glm::vec3& center, const glm::vec3& dimensions);
	Collider(const glm::vec3& center, const glm::vec3& dimensions);
	
	bool contains(Collider* other);
	glm::vec3 check_collision(Collider* other);
	void set_center(const glm::vec3& cen);
	glm::vec3 old_check_collision(Collider* other);
	bool check_ray_collision(glm::vec3 origin, glm::vec3 dir, glm::vec3& hitPosition);
	bool check_point_collision(glm::vec3 point);

	void setParentPlayer(ServerPlayer* player);
};
#include "ServerPlayer.h"