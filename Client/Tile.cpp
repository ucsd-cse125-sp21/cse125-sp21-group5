#include "Tile.h"
#include <ctime>

Tile::Tile(std::string modelPath, int numTree, float minPt, float maxPt) {
}

Tile::Tile(float gridI, float gridJ, float width, float height, int numTrees)
{
	// Create tile transform
	tileT = new Transform(glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(gridI * width, 0.0f, gridJ * height));

	// Create floor
	floorT = new Transform(glm::vec3(1.0f, 0.01f, 2.0f), glm::vec3(0.0f), glm::vec3(0.0f));
	Model* floorM = new Model("res/models/head2.dae");
	
	// Build scene graph
	tileT->add_child(floorT);
	floorT->add_child(floorM);

	// Load trees
	for (int i = 0; i < numTrees; i++) {
		// Pick a random height and location
		float tree_height = (float)rand() / (float)RAND_MAX;
		float tree_x = (float)rand() / (float)RAND_MAX - 0.5f;
		float tree_z = (float)rand() / (float)RAND_MAX - 0.5f;

		// Create transform at that position
		Transform* treeT =
			new Transform(glm::vec3(0.1f, tree_height/2.0f, 0.1f), glm::vec3(0.0f), glm::vec3(10.0f * tree_x, 0.0f, 10.0f * tree_z * height));
		Model* treeM = new Model("res/models/head2.dae");

		// Add to scene graph
		treeT->add_child(treeM);
		tileT->add_child(treeT);
	}
}

Tile::~Tile() {
	delete tileT;
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
void Tile::draw(const glm::mat4& parent_transform, const glm::mat4& view) {
	tileT->draw(parent_transform, view);
}