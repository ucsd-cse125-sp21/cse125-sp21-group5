#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

Model::Model(std::string modelPath)
{
	// loads the mesh into into position, normal, and index vectors
	loadModel(modelPath);

	// Generate a vertex array (VAO) and two vertex buffer objects (VBO).
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_positions);
	glGenBuffers(1, &VBO_normals);

	// Bind to the VAO.
	glBindVertexArray(VAO);

	// Bind to the first VBO - We will use it to store the vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind to the second VBO - We will use it to store the normals
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* normals.size(), normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Generate EBO, bind the EBO to the bound VAO and send the data
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::uvec3) * triangles.size(), triangles.data(), GL_STATIC_DRAW);

	// Unbind the VBOs.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Model::~Model()
{
	// Delete the VBOs and the VAO.
	glDeleteBuffers(1, &VBO_positions);
	glDeleteBuffers(1, &VBO_normals);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void Model::update()
{
}

void Model::draw(const glm::mat4& modelMtx, const glm::mat4& viewProjMtx, GLuint shader)
{

	// actiavte the shader program 
	glUseProgram(shader);

	// get the locations and send the uniforms to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, GL_FALSE, glm::value_ptr(viewProjMtx));
	//glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(rotatedModelMtx));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(modelMtx));
    //glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);

	// Bind the VAO
	glBindVertexArray(VAO);

	glDisable(GL_CULL_FACE);

	// draw the points using triangles, indexed with the EBO
	glDrawElements(GL_TRIANGLES, triangles.size() * 3, GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

//Using assimp's library
void Model::loadModel(std::string modelPath)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

	if (!scene) {
		std::cerr << "Failed to load model file " << modelPath << std::endl;
		return;
	}

	for (int meshidx = 0; meshidx < scene->mNumMeshes; meshidx++) {
		aiMesh* mesh = scene->mMeshes[meshidx];

		for (int vertidx = 0; vertidx < mesh->mNumVertices; vertidx++) {
			glm::vec3 vertex = glm::vec3(
				mesh->mVertices[vertidx].x,
				mesh->mVertices[vertidx].y,
				mesh->mVertices[vertidx].z
			);
			vertices.push_back(vertex); // store the vertices into the list
		}

		for (int normidx = 0; normidx < mesh->mNumVertices; normidx++) {
			glm::vec3 norm = glm::vec3(
				mesh->mNormals[normidx].x,
				mesh->mNormals[normidx].y,
				mesh->mNormals[normidx].z
			);
			normals.push_back(glm::normalize(norm)); // store the normals, the vector ortho to face of triangle, help with the color in shading
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
}
