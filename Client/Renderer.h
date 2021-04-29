#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <gl/glew.h>

#include "Camera.h"


#define NUM_POINT_LIGHTS 4

class PointLight {
public:
	glm::vec3 position;
	glm::vec3 color;

	// attenuation constants
	float constant, linear, quadratic;

	PointLight(glm::vec3 position, glm::vec3 color);
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

	std::vector<PointLight> mPointLights;

	void setCamera(Camera* camera);

	void addLight(PointLight light);
	void bindToShader(GLuint shader);
	

private:
	// we don't want to make any new renderers outside of the singleton
	Renderer() {}
};

