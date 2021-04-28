#pragma once


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Event
{
public:
	float dirX, dirY, dirZ;
	float speed;
	float posX, posY, posZ;

	Event(glm::vec3 dir, float speed, glm::vec3 position);
	Event();

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& dirX;
		ar& dirY;
		ar& dirZ;
		ar& speed;
		ar& posX;
		ar& posY;
		ar& posZ;


	}
};
