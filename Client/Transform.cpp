#include "Transform.h"

Transform::Transform()
{
	// Default identity matrix
	this->transform = glm::mat4(1.0f);
	this->scale = glm::vec3(1.0f);
	this->translation = glm::vec3(0.0f);
	this->rotation = glm::vec3(0.0f);
}

Transform::Transform(const glm::vec3& scale,
					 const glm::vec3& rotation,
					 const glm::vec3& translation)
{
	// Copy over variables
	this->scale = scale;
	this->rotation = rotation;
	this->translation = translation;

	// Build transform matrix
	create_transformation_matrix();
}

Transform::Transform(const Transform* transform)
{
	// Deep copy transformation matrix
	this->transform = transform->transform;
}

Transform::Transform(const vector<float>& transform)
{
	// TODO: how do i set scale, rotation, transform?
	// Build transformation matrix
	this->transform = glm::mat4(1.0f);
	this->scale = glm::vec3(1.0f);
	this->translation = glm::vec3(0.0f);
	this->rotation = glm::vec3(0.0f);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			this->transform[i][j] = transform[i + j];
		}
	}
}

Transform::~Transform()
{
	for (Node* child : children)
	{
		delete child;
	}
}

void Transform::translate(const glm::vec3& translation)
{
	//this->create_transformation_matrix();
	this->translation += translation;
	transform = glm::translate(transform, translation);
}

void Transform::setTranslate(const glm::vec3& translation)
{
	this->translation = translation;
	create_transformation_matrix();
}

void Transform::rotate(const float& angle, const glm::vec3& axis)
{
	this->rotation += angle * axis;
	transform = glm::rotate(transform, glm::radians(angle), axis);
}

void Transform::setRotate(const glm::vec3& rotation)
{
	this->rotation = rotation;
	create_transformation_matrix();
}

void Transform::rescale(const glm::vec3& scale)
{
	this->scale *= scale;
	transform = glm::scale(transform, scale);
}

void Transform::setScale(const glm::vec3& scale)
{
	this->scale = scale;
	create_transformation_matrix();
}

void Transform::create_transformation_matrix()
{
	// Build transformation matrix
	transform = glm::mat4(1.0f);

	// (rotation * (scale * model)) + translation
	transform = glm::scale(transform, scale);
	transform = glm::rotate(transform, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::translate(transform, translation);
}

void Transform::draw(const glm::mat4& parent_transform, const glm::mat4& view)
{
	for (Node* child : this->children)
	{
		child->draw(parent_transform * this->transform, view);
	}
}

void Transform::add_child(Node* child)
{
	children.push_back(child);
}