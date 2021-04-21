#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <cstdio>

#include "stb_image.h"

Model::Model(std::string modelPath)
{
	// loads the mesh into into position, normal, and index vectors
	loadModel(modelPath);

	// Initialize model
	this->model = glm::mat4(1.0f);

}

Model::~Model()
{
	// Delete the VBOs and the VAO.
	glDeleteBuffers(1, &VBO_positions);
	glDeleteBuffers(1, &VBO_normals);
	glDeleteBuffers(1, &VBO_texCoords);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);

	glDeleteTextures(1, &TEX_diffuse);
}

void Model::update()
{
}

void Model::move(glm::vec3& dir, float deltaTime)
{
	model = glm::translate(model, deltaTime * dir);
}


void Model::draw(const glm::mat4& modelMtx, const glm::mat4& viewProjMtx, GLuint shader)
{
	// activate the shader program 
	glUseProgram(shader);

	// get the locations and send the uniforms to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, GL_FALSE, glm::value_ptr(viewProjMtx));
	//glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(rotatedModelMtx));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(modelMtx));
    //glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);

	// bind the texture
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TEX_diffuse);

	// Bind the VAO
	glBindVertexArray(VAO);

	glDisable(GL_CULL_FACE);

	// draw the points using triangles, indexed with the EBO
	glDrawElements(GL_TRIANGLES, triangles.size() * 3, GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

// Using assimp's library
void Model::loadModel(std::string modelPath)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

	if (!scene) {
		std::cerr << "Failed to load model file " << modelPath << std::endl;
		return;
	}

	// Start loading material textures

	aiMaterial* mat = scene->mMaterials[0];
	aiString path;
	aiReturn texFound = mat->GetTexture(aiTextureType_DIFFUSE, 0, &path);

	char buff[100];
	snprintf(buff, sizeof(buff), "res/textures/%s", path.C_Str());
	std::cout << "Loading from " << buff << std::endl;

	int width, height, numChannels;
	unsigned char* data = stbi_load(buff, &width, &height, &numChannels, 0);

	glGenTextures(1, &TEX_diffuse);
	glBindTexture(GL_TEXTURE_2D, TEX_diffuse);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	if (data) {
		// we need to handle images with transparency slightly differently from regular 3 channel images
		if (numChannels == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		if (numChannels == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}

		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	// End loading material textures

	// Start loading mesh geometry
	for (int meshidx = 0; meshidx < scene->mNumMeshes; meshidx++) {
		aiMesh* mesh = scene->mMeshes[meshidx];

		// triangles are in reference to the MESH's vertices, not all the ones for the scene.
		int meshFirstVertex = vertices.size();

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
				mesh->mFaces[faceidx].mIndices[0] + meshFirstVertex,
				mesh->mFaces[faceidx].mIndices[1] + meshFirstVertex,
				mesh->mFaces[faceidx].mIndices[2] + meshFirstVertex
			);
			triangles.push_back(triangle); //store the indexes to the vertices array.
		}
		
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
