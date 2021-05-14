#pragma once


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Event
{
public:
	glm::vec3 dPos;
	float dYaw, dPitch;

	Event(glm::vec3 dPos, float dYaw, float dPitch);
	Event();

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& dPos.x;
		ar& dPos.y;
		ar& dPos.z;
		ar& dYaw;
		ar& dPitch;
	}
};
