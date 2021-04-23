#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <assimp/mesh.h>

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
	std::vector<glm::uvec3> triangles;  // the triange indices that make up the mesh
	std::vector<glm::vec2>	texCoords;  // the UV coordinates for the textures

	int materialIdx;

	Mesh(aiMesh* mesh);
	~Mesh();

	void draw();
};

