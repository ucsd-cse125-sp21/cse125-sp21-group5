#include "MapState.h"
#include <iostream>

MapState::MapState() {


}

// TODO: necessary?
void MapState::add(glm::mat4 transform) {
	vector<float> transformArray(16);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			transformArray[4 * i + j] = transform[i][j];
		}
	}

	//transforms.push_back(transformArray);
}