#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Node
{
public:
	virtual void draw(GLuint shader, glm::mat4 parent_transform);
};