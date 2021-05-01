#pragma once


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Event
{
public:
	glm::vec3 pos;
	float yaw, pitch;

	Event(glm::vec3 pos, float yaw, float pitch);
	Event();

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& pos.x;
		ar& pos.y;
		ar& pos.z;
		ar& yaw;
		ar& pitch;
	}
};
