#include "Collider.h"
#include <glm/gtx/string_cast.hpp>

Collider::Collider(const glm::vec3& center, const glm::vec3& dimensions)
{
	// Define center
	cen = center;
	dim = dimensions;
	min = cen - dim / 2.0f;
	max = cen + dim / 2.0f;
}

// this functions checks if the box contains another box
// this is only useful in the quadtree calculation of insertion 
bool Collider::contains(Collider* p) {
	return (p->cen.x > this->cen.x - this->dim.x &&
			p->cen.x < this->cen.x + this->dim.x &&
			p->cen.y > this->cen.y - this->dim.y &&
			p->cen.y < this->cen.y + this->dim.y &&
			p->cen.z > this->cen.z - this->dim.z &&
			p->cen.z < this->cen.z + this->dim.z);
}


glm::vec3 Collider::check_collision(Collider* other) {
	// TODO: Replace this with min and max corner vectors
	/*
	bool x = ((this->cen.x - this->dim.x / 2.0f) < (other->cen.x + other->dim.x / 2.0f)) &&
			 ((this->cen.x + this->dim.x / 2.0f) > (other->cen.x - other->dim.x / 2.0f));
	bool y = ((this->cen.y - this->dim.y / 2.0f) < (other->cen.y + other->dim.y / 2.0f)) &&
			 ((this->cen.y + this->dim.y / 2.0f) > (other->cen.y - other->dim.y / 2.0f));
	bool z = ((this->cen.z - this->dim.z / 2.0f) < (other->cen.z + other->dim.z / 2.0f)) &&
			 ((this->cen.z + this->dim.z / 2.0f) > (other->cen.z - other->dim.z / 2.0f));
	*/

	bool isColliding = glm::all(glm::lessThanEqual(this->min, other->max)) &&
					   glm::all(glm::lessThanEqual(other->min, this->max));

	/*
	std::cerr << "this->cen = " << glm::to_string(this->cen) << std::endl;
	std::cerr << "this->dim = " << glm::to_string(this->dim) << std::endl;
	std::cerr << "other->cen = " << glm::to_string(other->cen) << std::endl;
	std::cerr << "other->dim = " << glm::to_string(other->dim) << std::endl;
	*/

	// If there was a collision 
	if (isColliding)
	{
		// Determine which face it's intersecting with
		glm::vec3 diff = this->cen - other->cen;
		glm::vec3 abs_diff = glm::abs(diff);

		// x is max
		if (abs_diff.x >= abs_diff.y && abs_diff.x >= abs_diff.z)
			return glm::vec3(0.0f, 1.0f, 1.0f);
		// y is max
		else if (abs_diff.y >= abs_diff.x && abs_diff.y >= abs_diff.z)
			return glm::vec3(1.0f, 0.0f, 1.0f);
		// z is max
		else
			return glm::vec3(1.0f, 1.0f, 0.0f);
	}

	// No collision
	return glm::vec3(0.0f);
}


bool Collider::check_ray_collision(glm::vec3 origin, glm::vec3 dir, glm::vec3& hitPosition)
{
	// pos = origin + k * dir

	// Find collision distances
	glm::vec3 k_min = (min - origin) / dir;
	glm::vec3 k_max = (max - origin) / dir;

	// Find actual collision
	glm::vec3 x_min_plane_collision = origin + k_min.x * dir;
	glm::vec3 y_min_plane_collision = origin + k_min.y * dir;
	glm::vec3 z_min_plane_collision = origin + k_min.z * dir;

	glm::vec3 x_max_plane_collision = origin + k_max.x * dir;
	glm::vec3 y_max_plane_collision = origin + k_max.y * dir;
	glm::vec3 z_max_plane_collision = origin + k_max.z * dir;

	// Check if any min/max plane collision is within box range
	bool hit_min_x = check_point_collision(x_min_plane_collision);
	bool hit_min_y = check_point_collision(y_min_plane_collision);
	bool hit_min_z = check_point_collision(z_min_plane_collision);

	bool hit_max_x = check_point_collision(x_max_plane_collision);
	bool hit_max_y = check_point_collision(y_max_plane_collision);
	bool hit_max_z = check_point_collision(z_max_plane_collision);

	bool didHit = hit_min_x || hit_min_y || hit_min_z || hit_max_x || hit_max_y || hit_max_z;

	hitPosition = glm::vec3(0);
	float hitLength = RAND_MAX;

	if (hit_min_x && glm::length(x_min_plane_collision - origin) < hitLength) {
		hitPosition = x_min_plane_collision;
		hitLength = glm::length(x_min_plane_collision - origin);
	}
	if (hit_min_y && glm::length(y_min_plane_collision - origin) < hitLength) {
		hitPosition = y_min_plane_collision;
		hitLength = glm::length(y_min_plane_collision - origin);
	}
	if (hit_min_z && glm::length(z_min_plane_collision - origin) < hitLength) {
		hitPosition = z_min_plane_collision;
		hitLength = glm::length(z_min_plane_collision - origin);
	}
	if (hit_max_x && glm::length(x_max_plane_collision - origin) < hitLength) {
		hitPosition = x_max_plane_collision;
		hitLength = glm::length(x_max_plane_collision - origin);
	}
	if (hit_max_y && glm::length(y_max_plane_collision - origin) < hitLength) {
		hitPosition = y_max_plane_collision;
		hitLength = glm::length(y_max_plane_collision - origin);
	}
	if (hit_max_z && glm::length(z_max_plane_collision - origin) < hitLength) {
		hitPosition = z_max_plane_collision;
		hitLength = glm::length(z_max_plane_collision - origin);
	}
	return didHit;
}

bool Collider::check_point_collision(glm::vec3 point)
{
	return glm::all(glm::lessThanEqual(min, point)) && glm::all(glm::lessThanEqual(point, max));
}


