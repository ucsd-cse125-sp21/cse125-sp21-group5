#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <assimp/anim.h>
#include <assimp/mesh.h>

#include "AssimpGLMHelpers.h"

class Bone
{
public:

	std::string boneName;

	//same size, each vertex of the mesh mapped to a weight
	std::vector<int> vertexIDList; //represent the index in the vertice list of mesh
	std::vector<float> weightList;

	glm::mat4 offsetBoneMatrix;

	aiBone* aibone;

	Bone(aiBone* b);
	~Bone();

};

