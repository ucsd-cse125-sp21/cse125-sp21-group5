#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>

#include "Transform.h"
#include "Model.h"

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

	std::vector<Model*> treeList;
	Transform tileTrans;

	

public:
	Tile(std::string modelPath, int numTree, float minPt, float maxPt); //this number should be squarable
	~Tile();

	void update(); 
	void loadTile();
	void draw(glm::mat4 worldMtx, glm::mat4& viewProjMtx, GLuint shader); //just need to pass the tile's transform down the line to the trees
};
