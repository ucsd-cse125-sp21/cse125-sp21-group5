#include "Transform.h"

Transform::Transform()
{
	this->rotation = glm::vec3(0.0f);
	this->scale = glm::vec3(1.0f);
	this->translation = glm::vec3(1.0f);
	this->transform = glm::mat4(1.0f);
}

Transform::Transform(glm::vec3 rotation, glm::vec3 scale, glm::vec3 translation)
{
	this->rotation = rotation;
	this->scale = scale;
	this->translation = translation;
	this->create_transformation_matrix();
}

Transform::Transform(Transform& transform)
{
	this->rotation = transform.rotation;
	this->scale = transform.scale;
	this->translation = transform.translation;
	this->transform = transform.transform;
}

Transform::~Transform()
{

}

void Transform::translate(glm::vec3 translation)
{
	this->translation += translation;
	this->create_transformation_matrix();
}

void Transform::rotate(float angle, glm::vec3 axis)
{
	this->rotation += angle * axis;
	this->create_transformation_matrix();
}

void Transform::rescale(glm::vec3 scale)
{
	this->scale *= scale;
	this->create_transformation_matrix();
}

void Transform::create_transformation_matrix()
{
	transform = glm::mat4(1.0f);
	transform = glm::scale(transform, scale);
	transform = glm::rotate(transform, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::translate(transform, translation);
}

void Transform::draw(GLuint shader, glm::mat4 parent_transform)
{
	for (Node* child : this->children)
	{
		child->draw(shader, parent_transform * this->transform);
	}
}