#pragma once


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Event
{
public:
	glm::vec3 dPos;
	float dYaw, dPitch;
	bool shooting;
	bool dab;

	bool jumping;
	int gun_idx;

	int playerClass;

	Event(glm::vec3 dPos, float dYaw, float dPitch, bool shooting, bool jumping, int playerClass, int gun_idx, bool dab);
	Event();

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& dPos.x;
		ar& dPos.y;
		ar& dPos.z;
		ar& dYaw;
		ar& dPitch;
		ar& shooting;
		ar& jumping;
		ar& playerClass;
		ar& gun_idx;
		ar& dab;
	}
};
