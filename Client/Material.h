#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <assimp/material.h>

class Material
{
public:
	GLuint shader;

	GLuint tex_diffuse;

	// sets this material as the currently active one
	void activate();
	void release();

	Material(std::string vertShaderPath, std::string fragShaderPath, aiMaterial* aiMat);
	~Material();
};


