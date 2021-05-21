#include "Mesh.h"

#include <iostream>
#include "Model.h"

Mesh::Mesh(aiMesh* mesh, Model* parentModel)
{
	materialIdx = mesh->mMaterialIndex;
	this->parentModel = parentModel;

	VAO = 0;
	VBO = 0;
	EBO = 0;

	for (int vertidx = 0; vertidx < mesh->mNumVertices; vertidx++) {
		glm::vec3 position = glm::vec3(
			mesh->mVertices[vertidx].x,
			mesh->mVertices[vertidx].y,
			mesh->mVertices[vertidx].z
		);

		glm::vec3 norm = glm::vec3(
			mesh->mNormals[vertidx].x,
			mesh->mNormals[vertidx].y,
			mesh->mNormals[vertidx].z
		);

		glm::vec2 texCoord = glm::vec2(
			mesh->mTextureCoords[0][vertidx].x,
			mesh->mTextureCoords[0][vertidx].y
		);

		Vertex vert;
		vert.position = position;
		vert.normal = norm;
		vert.texCoord = texCoord;
		vertices.push_back(vert);
	}

	for (int faceidx = 0; faceidx < mesh->mNumFaces; faceidx++) {

		glm::uvec3 triangle = glm::vec3(
			mesh->mFaces[faceidx].mIndices[0],
			mesh->mFaces[faceidx].mIndices[1],
			mesh->mFaces[faceidx].mIndices[2]
		);
		triangles.push_back(triangle); //store the indexes to the vertices array.
	}
}

void Mesh::setupOpenGL() {
	// Generate a vertex array (VAO) and two vertex buffer objects (VBO).
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind to the VAO.
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	// vertex normals
	glEnableVertexAttribArray(1);	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
	// set up bone IDs
	glEnableVertexAttribArray(3);	
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, boneIDs));
	// set up bone weights
	glEnableVertexAttribArray(4);	
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, boneWeights));


	// Generate EBO, bind the EBO to the bound VAO and send the data
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uvec3) * triangles.size(), &triangles[0], GL_STATIC_DRAW);

	// Unbind the VBOs.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	// Delete the VBOs and the VAO.
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);

	for (int i = 0; i < boneList.size(); i++) {
		delete boneList.at(i);
	}
}

void Mesh::draw()
{
	glBindVertexArray(VAO);
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glDrawElements(GL_TRIANGLES, triangles.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
