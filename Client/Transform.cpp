#include "Transform.h"

Transform::Transform()
{
	// Default identity matrix
	this->transform = glm::mat4(1.0f);

	// Create collider object
	//this->collider = new Collider(glm::vec3(0.0f), 5.0f);
}

Transform::Transform(const glm::vec3& scale,
					 const glm::vec3& rotation,
					 const glm::vec3& translation/*
					 const Collider::Type type*/)
{
	// Build transformation matrix
	transform = glm::mat4(1.0f);

	// (rotation * (scale * model)) + translation
	transform = glm::scale(transform, scale);
	transform = glm::rotate(transform, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::translate(transform, translation);

	// Create collider object
	this->collider = new BoxCollider(translation, glm::vec3(1.0f)*5.0f);
}

Transform::Transform(const Transform* transform)
{
	// Deep copy transformation matrix
	this->transform = transform->transform;
	this->collider = transform->collider;
}

Transform::~Transform()
{
	for (Node* child : children)
	{
		delete child;
	}
	delete collider;
}

void Transform::translate(const glm::vec3& translation)
{
	//this->translation += translation;
	//this->create_transformation_matrix();
	transform = glm::translate(transform, translation);
	collider->center += translation;
}

void Transform::rotate(const float& angle, const glm::vec3& axis)
{
	//this->rotation += angle * axis;
	//this->create_transformation_matrix();
	transform = glm::rotate(transform, glm::radians(angle), axis);
}

void Transform::scale(const glm::vec3& scale)
{
	//this->scale *= scale;
	//this->create_transformation_matrix();
	transform = glm::scale(transform, scale);
	//collider->radius *= scale.x;
}

void Transform::create_transformation_matrix()
{
	// TODO: probably delete this
	/* Not sure if we'd have to rebuild transformation matrix everytime
	transform = glm::mat4(1.0f);
	transform = glm::scale(transform, scale);
	transform = glm::rotate(transform, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::translate(transform, translation);
	*/
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