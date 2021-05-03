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

/*
 * Handles one geometry mesh for a model
 */
class Mesh
{
public:
	GLuint VAO;
	GLuint VBO_positions, VBO_normals, VBO_texCoords, EBO;

	std::vector<glm::vec3>	vertices;	// the vertices, tells position of the points
	std::vector<glm::vec3>	normals;	// the vertex normals
	std::vector<glm::uvec3> triangles;  // the triangle indices that make up the mesh
	std::vector<glm::vec2>	texCoords;  // the UV coordinates for the textures

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

