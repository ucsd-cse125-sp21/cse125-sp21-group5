#include "Collider.h"
#include "glm/gtx/string_cast.hpp"
#include <iostream>
#include <algorithm>

// TODO: potentailly radius x,y,z
//SphereCollider::SphereCollider(const glm::vec3& center, const float& radius)
//{
//	this->center = center;
//	this->radius = radius;
//}

Collider::Collider(const glm::vec3& center, const glm::vec3& dimensions)
{
	// Define center
	this->center = center;
	this->length = dimensions.x;
	this->width = dimensions.y;
	this->height = dimensions.z;
			/*
		   7--------6
		  /        /|
		 /        / |
		3--------2  |
		|        |  |
		|   5----|--4
		|  /     | /
		| /      |/
		1--------0
			*/

	// Build eight points
	// TODO: Useless
	points[0] = glm::vec3(center.x - dimensions.x / 2,
						  center.y - dimensions.y / 2,
		                  center.z - dimensions.z / 2);
	points[1] = glm::vec3(center.x + dimensions.x / 2,
		                  center.y - dimensions.y / 2,
		                  center.z - dimensions.z / 2);
	points[2] = glm::vec3(center.x - dimensions.x / 2,
		                  center.y + dimensions.y / 2,
		                  center.z - dimensions.z / 2);
	points[3] = glm::vec3(center.x + dimensions.x / 2,
		                  center.y + dimensions.y / 2,
		                  center.z - dimensions.z / 2);
	points[4] = glm::vec3(center.x - dimensions.x / 2,
		                  center.y - dimensions.y / 2,
		                  center.z + dimensions.z / 2);
	points[5] = glm::vec3(center.x + dimensions.x / 2,
		                  center.y - dimensions.y / 2,
		                  center.z + dimensions.z / 2);
	points[6] = glm::vec3(center.x - dimensions.x / 2,
		                  center.y + dimensions.y / 2,
		                  center.z + dimensions.z / 2);
	points[7] = glm::vec3(center.x + dimensions.x / 2,
		                  center.y + dimensions.y / 2,
		                  center.z + dimensions.z / 2);

	// Define 6 faces

}

// this functions checks if the box contains another box
// this is only useful in the quadtree calculation of insertion 
bool Collider::contains(Collider* p) {
	return (p->center.x > this->center.x - this->length &&
		p->center.x < this->center.x + this->length &&
		p->center.y > this->center.y - this->width &&
		p->center.y < this->center.y + this->width &&
		p->center.z > this->center.z - this->height &&
		p->center.z < this->center.z + this->height);
}


glm::vec3 Collider::intersects(Collider* range) {
	// Check x-y-z plane
	bool xy = !(
		range->center.x - range->length > this->center.x + this->length ||
		range->center.x + range->length < this->center.x - this->length ||
		range->center.y - range->width > this->center.y + this->width ||
		range->center.y + range->width < this->center.y - this->width
		);
	bool yz = !(
		range->center.y - range->width > this->center.y + this->width ||
		range->center.y + range->width < this->center.y - this->width ||
		range->center.z - range->height > this->center.z + this->height ||
		range->center.z + range->height < this->center.z - this->height
		);
	bool zx = !(
		range->center.z - range->height > this->center.z + this->height ||
		range->center.z + range->height < this->center.z - this->height ||
		range->center.x - range->length > this->center.x + this->length ||
		range->center.x + range->length < this->center.x - this->length
		);

	// If intersecting
	if (xy && yz && zx)
	{
		// Determine which face it's intersecting with
		glm::vec3 diff = this->center - range->center;
		glm::vec3 abs_diff = glm::abs(diff);

		// x is max
		if (abs_diff.x >= abs_diff.y && abs_diff.x >= abs_diff.z)
		{
			//float dir = (diff.x >= 0.0f) ? 1.0f : -1.0f;
			return glm::vec3(0.0f, 1.0f, 1.0f);
		}
		// y is max
		else if (abs_diff.y >= abs_diff.x && abs_diff.y >= abs_diff.z)
		{
			return glm::vec3(1.0f, 0.0f, 1.0f);
		}
		// z is max
		else
		{
			return glm::vec3(1.0f, 1.0f, 0.0f);
		}
	}
	return glm::vec3(0.0f);
}
//
//bool BoxCollider::contains(SphereCollider* p) {
//	bool furthestX = p->center.x + p->radius;
//	bool furthestY = p->center.y + p->radius;
//	bool furthestZ = p->center.z + p->radius;
//	return false;
//}

//bool Collider::contains(Collider* p) {
//	return false;
//}

//bool BoxCollider::check_collision(BoxCollider* other) {
//	//std::cerr << "Checking collision between box and box" << std::endl;
//	return false;
//}

//bool SphereCollider::check_collision(SphereCollider* other)
//{
//	float center_diff = glm::distance(this->center, other->center);
//	bool tmp = center_diff <= this->radius + other->radius;
//	if (tmp)
//		std::cerr << "Sphere & Sphere Collision Detected!" << std::endl;
//	return tmp;
//}

//bool SphereCollider::check_collision(BoxCollider* other)
//{
//
//	return false;
//}
//
//bool SphereCollider::check_collision(PlaneCollider* other)
//{
//	return false;
//}
