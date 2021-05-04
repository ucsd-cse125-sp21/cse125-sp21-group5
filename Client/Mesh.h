#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <assimp/mesh.h>
#include <assimp/anim.h>

#include "Bone.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
	
	int boneIDs[MAX_BONE_INFLUENCE];
	float boneWeights[MAX_BONE_INFLUENCE];
};

/*
 * Handles one geometry mesh for a model
 */
class Mesh
{
public:
	GLuint VAO;
	GLuint VBO, EBO;


	std::vector<Vertex> vertices;

	//std::vector<glm::vec3>	oldVertices;	// the vertices, tells position of the points
	//std::vector<glm::vec3>	normals;	// the vertex normals
	//std::vector<glm::vec2>	texCoords;  // the UV coordinates for the textures

	std::vector<glm::uvec3> triangles;  // the triangle indices that make up the mesh

	//each mesh might have an array of bones that affect it. Each bone has a list of the vertices and the weights
	//associated with that vertices
	// 	   Mesh 1 (vertex and weight) Pretend Mesh 1 has 2 vertices.
	//Ex: BoneHead v1 : 0.5, V2: 0.4
	//	  BoneNeck v1 : 0.5  V2: 0.6
	// Notice the total weight of each vertex add up to 1.
	std::vector<Bone*> boneList;

	int materialIdx;

	Mesh(aiMesh* mesh);
	~Mesh();

	void draw();
};

