#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

class Model
{
private:
	GLuint VAO;
	GLuint VBO_positions, VBO_normals, EBO;

	// TODO replace with a Transform class
	glm::mat4 model; //the local 

	//maybe have a transform object, for parent and world

	std::vector<glm::vec3> vertices;  //the vertices, tells position of the points
	std::vector<glm::vec3> normals;
	std::vector<glm::uvec3> triangles;

	void loadModel(std::string modelPath); //read in the file, load the model's data

public:
	Model(std::string modelPath);
	~Model();

	void update();
	void draw(const glm::mat4& modelMtx, const glm::mat4& viewProjMtx, GLuint shader);
};

