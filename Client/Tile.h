#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>

#include "Model.h"
#include "Transform.h"

class Tile
{
private:
	GLuint VAO;
	GLuint VBO_positions, VBO_normals, EBO;

	// The positions of the 4 corner of the tile
	glm::vec3 A; 
	glm::vec3 B;
	glm::vec3 C;
	glm::vec3 D;

	glm::vec3 AB;
	glm::vec3 AC;

	// How to break up the tile to place the trees randomly
	int stratify; 

	Transform* floorT;
	std::vector<Node*> treeList;

public:
	Transform* tileT;

	Tile(std::string modelPath, int numTree, float minPt, float maxPt); //this number should be squarable
	Tile(float gridI, float gridJ, float width, float height, int numTrees);
	~Tile();

	void update(); 
	void loadTile();
	void draw(const glm::mat4& parent_transform, const glm::mat4& view); //just need to pass the tile's transform down the line to the trees
};
