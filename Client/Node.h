#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Node
{
public:
	virtual void draw(const glm::mat4& parent_transform, const glm::mat4& view) = 0;
	virtual void update(float deltaTime) = 0;
};