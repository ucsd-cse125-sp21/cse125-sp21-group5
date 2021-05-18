#include "MapState.h"
#include <iostream>

MapState::MapState() {}

// Create piece of specified scale, rotation, and translation
MapState::MapState(int seed) {
	this->tileSeed = seed;
};