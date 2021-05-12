#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

class Collider
{
public:
	// Variables that colliders should have
	glm::vec3 center;

	// TODO think of a better way to do this
	float length;
	float width;
	float height;

	enum class Type
	{
		sphere,
		box,
		plane
	};

	Collider() {}
	Collider::Type type;

	/*virtual bool check_collision(Collider* other) = 0*/
	//virtual bool check_collision(BoxCollider* other) = 0;
};

// TODO: Is this bad? Should we just move to new files
class SphereCollider;
class BoxCollider;
class PlaneCollider;

class SphereCollider : public Collider
{
public:
	glm::vec3 center;
	float radius;
	Collider::Type type = Collider::Type::sphere; // TODO: necessary?

	SphereCollider(const glm::vec3& center, const float& radius);

	
	bool check_collision(BoxCollider* other);
	bool check_collision(SphereCollider* other);
	bool check_collision(PlaneCollider* other);

};

class BoxCollider : public Collider
{
public:
	// The eight corners that make up a box
	glm::vec3 points[8];
	//glm::vec3 center;
	float length;
	float width;
	float height;
	
	// Need a default constructor because it needs to be used by quadtree
	BoxCollider(){}
	BoxCollider(const glm::vec3& center, const glm::vec3& dimensions);
	bool check_collision(BoxCollider* other);
	//bool check_collision(Collider* other);
	//bool check_collision(Collider* other);
    bool contains(BoxCollider* p);
	bool contains(Collider* p);
	bool contains(SphereCollider* p);
	glm::vec3 intersects(BoxCollider* range);

	// Helper functions to determine if box contains sphere 
	//float plane_distance()
};

class PlaneCollider : public Collider
{
public:
	// The four corners that make up a plane
	glm::vec3 points[4];

	PlaneCollider(const glm::vec3& a,
				  const glm::vec3& b,
				  const glm::vec3& c,
				  const glm::vec3& d);
	bool check_collision(Collider* other);
};
