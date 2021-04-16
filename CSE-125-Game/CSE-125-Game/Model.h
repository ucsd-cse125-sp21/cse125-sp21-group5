#ifndef _MODEL_H_
#define _MODEL_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>

class Model
{
private:
	GLuint VAO;
	GLuint VBO_positions, VBO_normals, EBO;

	// TODO replace with a Transform class
	glm::mat4 model; //the local model transform
	glm::vec3 color;

	//maybe have a transform object, for parent and world

	std::vector<glm::vec3> vertices;  //the vertices, tells position of the points
	std::vector<glm::vec3> normals;
	std::vector<glm::uvec3> triangles;

	void loadModel(std::string modelPath); //read in the file, load the model's data

public:
	Model(std::string modelPath);
	~Model();

	glm::mat4 getModel() { return model; }
	glm::vec3 getColor() { return color; }

	void update();
	void draw(const glm::mat4& modelMtx, const glm::mat4& viewProjMtx, GLuint shader);
	void move(glm::vec3& dir, float deltaTime);
};

#endif