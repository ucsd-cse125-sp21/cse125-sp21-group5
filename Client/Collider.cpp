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
