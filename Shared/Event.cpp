#include "Event.h"

Event::Event(glm::vec3 dir, float speed, glm::vec3 position) {
	this->dirX = dir.x;
	this->dirY = dir.y;
	this->dirZ = dir.z;
	this->speed = speed;
	this->posX = position.x;
	this->posY = position.y;
	this->posZ = position.z;

}

Event::Event() {
	this->dirX = 0;
	this->dirY = 0;
	this->dirZ = 0;
	this->speed = 0;
	this->posX = 0;
	this->posY = 0;
	this->posZ = 0;
}