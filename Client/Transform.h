#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>

class Transform
{
private:
	
	glm::vec3 translation = glm::vec3(0);	// the vertices, tells position of the points
	glm::vec3 rotation = glm::vec3(0);	// the vertex normals
	float scale = 1.0f;  // the triange indices that make up the mesh

	glm::mat4 T = glm::mat4(1.0f); //translation matrix
	glm::mat4 R = glm::mat4(1.0f); //rotation matrix
	glm::mat4 S = glm::mat4(1.0f); //scale matrixs
	

public:
	Transform();
	~Transform();

	glm::mat4 translate(glm::vec3 t);
	glm::mat4 rotate(glm::vec3 axis, float angle); // in radians
	glm::mat4 resize(float scaleFactor);
};
