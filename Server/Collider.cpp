#include "Collider.h"

// TODO: potentailly radius x,y,z
SphereCollider::SphereCollider(const glm::vec3& center, const float& radius)
{
	this->center = center;
	this->radius = radius;
}

BoxCollider::BoxCollider(const glm::vec3& center, const glm::vec3& dimensions)
{
	// Define center
	this->center = center;
	this->length = dimensions.x;
	this->width = dimensions.y;
	this->height = dimensions.z;
			/*
		   6--------4
		  /        /|
		 /        / |
		7--------5  |
		|        |  |
		|   2----|--0
		|  /     | /
		| /      |/
		3--------1
			*/

	// Build eight points
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
}

// this functions checks if the box contains another box
// this is only useful in the quadtree calculation of insertion 
bool BoxCollider::contains(BoxCollider* p) {
	return (p->center.x > this->center.x - this->length &&
		p->center.x < this->center.x + this->length &&
		p->center.y > this->center.y - this->width &&
		p->center.y < this->center.y + this->width &&
		p->center.z > this->center.z - this->height &&
		p->center.z < this->center.z + this->height);
}


bool BoxCollider::intersects(BoxCollider* range) {
	// Check x-y-z plane
	//std::cout << range->center.x - range->length << std::endl;
	//std::cout << this->center.x + this->length << std::endl;
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
	//if (xy && yz && zx) std::cerr << "intersected" << std::endl;
	return xy && yz && zx;
}

bool BoxCollider::contains(SphereCollider* p) {
	bool furthestX = p->center.x + p->radius;
	bool furthestY = p->center.y + p->radius;
	bool furthestZ = p->center.z + p->radius;
}

bool BoxCollider::contains(Collider* p) {
	return false;
}

bool BoxCollider::check_collision(BoxCollider* other) {
	//std::cerr << "Checking collision between box and box" << std::endl;
	return this->intersects(other);
}

bool SphereCollider::check_collision(SphereCollider* other)
{
	float center_diff = glm::distance(this->center, other->center);
	bool tmp = center_diff <= this->radius + other->radius;
	if (tmp)
		std::cerr << "Sphere & Sphere Collision Detected!" << std::endl;
	return tmp;
}

bool SphereCollider::check_collision(BoxCollider* other)
{

	return false;
}

bool SphereCollider::check_collision(PlaneCollider* other)
{
	return false;
}
