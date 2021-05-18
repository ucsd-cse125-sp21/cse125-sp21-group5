#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <gl/glew.h>

#include "Camera.h"


#define NUM_POINT_LIGHTS 32

class PointLight {
public:
	glm::vec3 mPosition;
	glm::vec3 mColor;

	// attenuation constants
	float mConstant, mLinear, mQuadratic;

	PointLight(glm::vec3 position, glm::vec3 color);
};

class DirectionalLight {
public:
	glm::vec3 mDirection;
	glm::vec3 mColor;

	DirectionalLight(glm::vec3 direction, glm::vec3 color) {
		mDirection = glm::normalize(direction);
		mColor = color;
	}

	DirectionalLight() {
		mDirection = glm::vec3(0, 1, 0);
		mColor = glm::vec3(0, 0, 0);
	}
};


/*
 * The global renderer. Right now this just needs to keep track of the scene lights that will
 * be applied to all the different shaders.
 */
class Renderer
{
public:

	// START SINGLETON STUFF
	static Renderer& get() {
		static Renderer instance;
		return instance;
	}

	// these are all needed to make sure we don't get copies of the renderer singleton
	Renderer(Renderer const&) = delete;
	void operator=(Renderer const&) = delete;
	// END SINGLETON STUFF

	Camera* mCamera;

	// fog parameters
	float fogDensity;
	float fogGradient;
	glm::vec3 fogColor;

	std::vector<PointLight> mPointLights;

	DirectionalLight mDirectionalLight;

	void setCamera(Camera* camera);

	void addPointLight(PointLight light);
	void addDirectionalLight(DirectionalLight light);
	void bindToShader(GLuint shader);

	// let renderer update anything it needs
	void update(float deltaTime);

private:
	// we don't want to make any new renderers outside of the singleton
	Renderer() {
		mCamera = nullptr;
		fogDensity = 0.0001;
		fogGradient = 1;
		fogColor = glm::vec3(0.2, 0.05, 0.15);
	}
};

