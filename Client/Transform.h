#pragma once
#include "Node.h"
#include <vector>
#include <string>

using namespace std;

class Transform : public Node
{
public:
	// Variables for positions
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::vec3 translation;
	glm::mat4 transform;

	// Keep children
	vector<Node*> children;

	// Default constructor
	Transform();

	// Specify everything constructor
	Transform(glm::vec3 rotation, glm::vec3 scale, glm::vec3 translation);

	// Copy constructor
	Transform(Transform& transform);


	~Transform();

	void translate(glm::vec3 translation);
	void rotate(float angle, glm::vec3 axis);
	void rescale(glm::vec3 scale);
	void create_transformation_matrix();
	void draw(GLuint shader, glm::mat4 parent_transform);
};
