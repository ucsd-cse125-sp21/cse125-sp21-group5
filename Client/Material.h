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

	// sets this material as the currently active one
	virtual void activate() = 0;

	// cleans up all bindings
	virtual void release() = 0;

	//Material(std::string vertShaderPath, std::string fragShaderPath, aiMaterial* aiMat);
};


class TexturedMaterial : public Material
{
public:
	GLuint tex_diffuse;

	void activate();
	void release();

	TexturedMaterial(aiMaterial* aiMat);
	~TexturedMaterial();
};

class DiffuseMaterial : public Material
{
public:
	glm::vec3 diffuseColor;
	GLuint colorLocation;

	void activate();
	void release();
	
	DiffuseMaterial(aiMaterial* aiMat);
	~DiffuseMaterial();
};

