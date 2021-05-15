#include "Event.h"

// Empty event
Event::Event() {
	this->dPos = glm::vec3(0);
	this->dYaw = 0;
	this->dPitch = 0;
	this->shooting = false;
}

// Movement update
Event::Event(glm::vec3 dPos, float dYaw, float dPitch, bool shooting) {
	this->dPos = dPos;
	this->dYaw = dYaw;
	this->dPitch = dPitch;
	this->shooting = shooting;
}