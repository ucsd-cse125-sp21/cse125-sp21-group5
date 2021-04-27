#include "Collider.h"

Collider::Collider(const glm::vec3& center, const float& radius)
{
	this->center = center;
	this->radius = radius;
	this->type = Collider::Type::sphere;
}

Collider::Collider(const glm::vec3& dimensions)
{

}

bool Collider::check_collision(Collider* other)
{
	if (this->type == Collider::Type::sphere)
	{
		if (other->type == Collider::Type::sphere)
		{
			float center_diff = glm::distance(this->center, other->center);
			bool tmp = center_diff <= this->radius + other->radius;
			if (tmp)
				std::cerr << "Sphere & Sphere Collision Detected!"<< std::endl;
			return tmp;
		}
		else if (other->type == Collider::Type::box)
		{

		}
		else if (other->type == Collider::Type::plane)
		{

		}
		else
		{
			// ??
		}
	}
	else if (this->type == Collider::Type::box)
	{
		if (other->type == Collider::Type::sphere)
		{

		}
		else if (other->type == Collider::Type::box)
		{

		}
		else if (other->type == Collider::Type::plane)
		{

		}
		else
		{
			// ??
		}
	}
	else if (this->type == Collider::Type::plane)
	{
		if (other->type == Collider::Type::sphere)
		{

		}
		else if (other->type == Collider::Type::box)
		{

		}
		else if (other->type == Collider::Type::plane)
		{

		}
		else
		{
			// ??
		}
	}


}