#pragma once
#include "Node.h"
#include <vector>
#include <string>

using namespace std;

class Transform : public Node
{
private:
	// Track children
	vector<Node*> children;

public:
	// Variables for positions
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::vec3 translation;
	glm::mat4 transform;

	// Default constructor
	Transform();

	// Specify everything constructor
	Transform(const glm::vec3& scale,
			  const glm::vec3& rotation,
			  const glm::vec3& translation);

	// Copy constructor
	Transform(const Transform* transform);

	// Weird constructor
	Transform(const vector<float>& transform);

	~Transform();

	void translate(const glm::vec3& translation);
	void rotate(const float& angle, const glm::vec3& axis);
	void rescale(const glm::vec3& scale);
	void create_transformation_matrix();

	void setTranslate(const glm::vec3& translation);
	void setRotate(const glm::vec3& rotation);
	void setScale(const glm::vec3& scale);

	void draw(const glm::mat4& parent_transform, const glm::mat4& view);
	void add_child(Node* child);
};
