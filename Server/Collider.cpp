#include "Collider.h"

Collider::Collider(const glm::vec3& center, const glm::vec3& dimensions)
{
	// Define center
	this->center = center;
	this->dim = dimensions;
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
	/*
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
	*/
}

// this functions checks if the box contains another box
// this is only useful in the quadtree calculation of insertion 
bool Collider::contains(Collider* p) {
	return (p->center.x > this->center.x - this->dim.x &&
			p->center.x < this->center.x + this->dim.x &&
			p->center.y > this->center.y - this->dim.y &&
			p->center.y < this->center.y + this->dim.y &&
			p->center.z > this->center.z - this->dim.z &&
			p->center.z < this->center.z + this->dim.z);
}


glm::vec3 Collider::check_collision(Collider* other) {
	// Intersection on xy plane
	bool xy = (
		other->center.x - other->dim.x <= this->center.x + this->dim.x &&
		other->center.x + other->dim.x >= this->center.x - this->dim.x &&
		other->center.y - other->dim.y <= this->center.y + this->dim.y &&
		other->center.y + other->dim.y >= this->center.y - this->dim.y
		);

	// Intersection on yz plane
	bool yz = !(
		other->center.y - other->dim.y > this->center.y + this->dim.y ||
		other->center.y + other->dim.y < this->center.y - this->dim.y ||
		other->center.z - other->dim.z > this->center.z + this->dim.z ||
		other->center.z + other->dim.z < this->center.z - this->dim.z
		);

	// Intersection on zx plane
	bool zx = !(
		other->center.z - other->dim.z > this->center.z + this->dim.z ||
		other->center.z + other->dim.z < this->center.z - this->dim.z ||
		other->center.x - other->dim.x > this->center.x + this->dim.x ||
		other->center.x + other->dim.x < this->center.x - this->dim.x
		);

	// If there was a collision 
	if (xy && yz && zx)
	{
		// Determine which face it's intersecting with
		glm::vec3 diff = this->center - other->center;
		glm::vec3 abs_diff = glm::abs(diff);

		// x is max
		if (abs_diff.x >= abs_diff.y && abs_diff.x >= abs_diff.z)
		{
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