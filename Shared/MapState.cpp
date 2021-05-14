#include "MapState.h"
#include <iostream>

// Creates unit piece at 0,0,0
MapPiece::MapPiece()
{
	this->scale = glm::vec3(1.0f);
	this->rotation = glm::vec3(0.0f);
	this->translation = glm::vec3(0.0f);
}

// Create piece of specified scale, rotation, and translation
MapPiece::MapPiece(const glm::vec3& scale,
				   const glm::vec3& rotation,
				   const glm::vec3& translation)
{
	this->scale = scale;
	this->rotation = rotation;
	this->translation = translation;
}

// Adds a MapPiece to the pieces
void MapState::addPiece(MapPiece& mp) {
	this->pieces.push_back(mp);
}