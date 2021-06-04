#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <assimp/material.h>

#include "AnimationPlayer.h"
#include "Renderer.h"

class Material
{
public:
	GLuint shader;

	GLuint viewProjLoc;
	GLuint viewPosLoc;
	GLuint viewDirLoc;
	GLuint modelLoc;

	GLuint fogColorLoc;
	GLuint fogDensityLoc;
	GLuint fogGradientLoc;

	GLuint sunLightDir;
	GLuint sunLightColor;

	GLuint numPointLights;
	GLuint numSpotLights;

	GLuint boneMatricesLoc[MAX_NUM_BONES];

	GLuint pointLightPosLoc[NUM_POINT_LIGHTS];
	GLuint pointLightColorLoc[NUM_POINT_LIGHTS];
	GLuint pointLightAttenConstLoc[NUM_POINT_LIGHTS];
	GLuint pointLightAttenLinearLoc[NUM_POINT_LIGHTS];
	GLuint pointLightAttenQuadLoc[NUM_POINT_LIGHTS];

	GLuint spotLightPosLoc[NUM_SPOT_LIGHTS];
	GLuint spotLightColorLoc[NUM_SPOT_LIGHTS];
	GLuint spotLightAttenConstLoc[NUM_SPOT_LIGHTS];
	GLuint spotLightAttenLinearLoc[NUM_SPOT_LIGHTS];
	GLuint spotLightAttenQuadLoc[NUM_SPOT_LIGHTS];
	GLuint spotLightAngleLoc[NUM_SPOT_LIGHTS];

	// sets this material as the currently active one
	virtual void activate() = 0;

	// cleans up all bindings
	virtual void release() = 0;

	void loadShaderLocations();
};


class TexturedMaterial : public Material
{
public:

	// UNIFORMS
	GLuint tex_diffuse;

	// sets this material as the currently active one
	void activate();
	void release();

	TexturedMaterial(aiMaterial* aiMat, std::string playerType);
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

