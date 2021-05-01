#include "Event.h"

Event::Event(glm::vec3 pos, float yaw, float pitch) {
	this->pos = pos;
	this->yaw = yaw;
	this->pitch = pitch;
}

Event::Event() {
	this->pos = glm::vec3(0);
	this->yaw = 0;
	this->pitch = 0;
}