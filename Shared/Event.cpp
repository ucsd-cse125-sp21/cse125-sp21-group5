#include "Event.h"

// TODO: possibly just send new position, so server doesn't have to calculate for us
Event::Event(glm::vec3 dir, float speed, glm::vec3 position) {
	this->dir = dir;
	this->speed = speed;
	this->position = position;
}
