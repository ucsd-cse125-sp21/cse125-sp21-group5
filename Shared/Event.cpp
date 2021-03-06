#include "Event.h"

// Empty event
Event::Event() {
	this->dPos = glm::vec3(0);
	this->dYaw = 0;
	this->dPitch = 0;
	this->shooting = false;
	this->jumping = false;
	this->gun_idx = 0;
	this->dab = false;
	this->isReady = false;
}

// Movement update
Event::Event(glm::vec3 dPos, float dYaw, float dPitch, bool shooting, bool jumping, int playerClass, int gun_idx, bool dab, bool isReady, bool isDisrespecting) {
	this->dPos = dPos;
	this->dYaw = dYaw;
	this->dPitch = dPitch;
	this->shooting = shooting;
	this->jumping = jumping;
	this->playerClass = playerClass;
	this->gun_idx = gun_idx;
	this->dab = dab;
	this->isReady = isReady;
	this->isDisrespecting = isDisrespecting;
}