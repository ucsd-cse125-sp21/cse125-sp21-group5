#include "Event.h"

// TODO: possibly just send new position, so server doesn't have to calculate for us
Event::Event(glm::vec3 dir, float speed, glm::vec3 position) {
	this->dirX = dir.x;
	this->dirY = dir.y;
	this->dirZ = dir.z;
	this->speed = speed;
	this->posX = position.x;
	this->posY = position.y;
	this->posZ = position.z;

}

Event::Event() {}