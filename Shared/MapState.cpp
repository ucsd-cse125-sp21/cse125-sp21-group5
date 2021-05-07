#include "MapState.h"

MapState::MapState() {


}

void MapState::add(glm::mat4 transform) {
	vector<float> transformArray;
	transformArray.reserve(16);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			transformArray[i + j] = transform[i][j];
		}
	}

	transforms.push_back(transformArray);
}