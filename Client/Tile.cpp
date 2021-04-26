#include "Tile.h"
#include <ctime>

Tile::Tile(std::string modelPath, int numTree, float minPt, float maxPt) {
	stratify = sqrt(numTree);

	// a representation of a quad
	A = glm::vec3(-minPt, 0, -maxPt); 
	B = glm::vec3(-minPt, 0, maxPt);
	C = glm::vec3(minPt, 0, -maxPt);
	D = glm::vec3(minPt, 0, maxPt);

	AB = B - A;
	AC = C - A;

	// Load trees
	for (int i = 0; i < numTree; i++) {
		Model* tree = new Model("res/models/head2.dae");
		treeList.push_back(tree);
	}
}

Tile::~Tile() {

}

void Tile::loadTile() {
	/*
	float counter = 0;
	srand((unsigned int)time(NULL));
	for (int i = 0; i < stratify; i++) {
		for (int j = 0; j < stratify; j++) {
			float u1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float u2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			glm::vec3 newPosition = A + ((j + u1) / stratify) * AB + ((i + u2) / stratify) * AC;
			treeList.at(counter)->move(newPosition);
			counter++;
		}
	}
	*/
}


void Tile::update() {

}

//the worldMtx will probably be the tile's localTransform matrix or something
void Tile::draw(glm::mat4 worldMtx, glm::mat4& viewProjMtx, GLuint shader) {
	/*
	for (int i = 0; i < treeList.size(); i++) {
		treeList.at(i)->draw(worldMtx, viewProjMtx, shader);
	}
	*/
}