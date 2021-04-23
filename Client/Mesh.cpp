#include "Mesh.h"

#include <iostream>

Mesh::Mesh(aiMesh* mesh)
{
	materialIdx = mesh->mMaterialIndex;

	for (int vertidx = 0; vertidx < mesh->mNumVertices; vertidx++) {
		glm::vec3 vertex = glm::vec3(
			mesh->mVertices[vertidx].x,
			mesh->mVertices[vertidx].y,
			mesh->mVertices[vertidx].z
		);
		vertices.push_back(vertex); // store the vertices into the list

		glm::vec3 norm = glm::vec3(
			mesh->mNormals[vertidx].x,
			mesh->mNormals[vertidx].y,
			mesh->mNormals[vertidx].z
		);
		normals.push_back(glm::normalize(norm)); // store the normals, the vector ortho to face of triangle, help with the color in shading

		glm::vec2 texCoord = glm::vec2(
			mesh->mTextureCoords[0][vertidx].x,
			mesh->mTextureCoords[0][vertidx].y
		);
		texCoords.push_back(texCoord);
	}

	for (int faceidx = 0; faceidx < mesh->mNumFaces; faceidx++) {

		glm::uvec3 triangle = glm::vec3(
			mesh->mFaces[faceidx].mIndices[0],
			mesh->mFaces[faceidx].mIndices[1],
			mesh->mFaces[faceidx].mIndices[2]
		);
		triangles.push_back(triangle); //store the indexes to the vertices array.
	}

	// Generate a vertex array (VAO) and two vertex buffer objects (VBO).
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_positions);
	glGenBuffers(1, &VBO_normals);
	glGenBuffers(1, &VBO_texCoords);

	// Bind to the VAO.
	glBindVertexArray(VAO);

	// Bind to the first VBO - We will use it to store the vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind to the second VBO - We will use it to store the normals
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind the third VBO - We will use it to store the texture uv coordinates
	glBindBuffer(GL_ARRAY_BUFFER, VBO_texCoords);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * texCoords.size(), texCoords.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);


	// Generate EBO, bind the EBO to the bound VAO and send the data
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uvec3) * triangles.size(), triangles.data(), GL_STATIC_DRAW);

	// Unbind the VBOs.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	// Delete the VBOs and the VAO.
	glDeleteBuffers(1, &VBO_positions);
	glDeleteBuffers(1, &VBO_normals);
	glDeleteBuffers(1, &VBO_texCoords);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void Mesh::draw()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, triangles.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
