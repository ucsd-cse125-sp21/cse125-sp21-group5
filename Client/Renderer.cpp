#include "Renderer.h"
#include "imgui.h"

#include "Shader.h"

#include <glm/gtx/string_cast.hpp>
#include "Material.h"


void Renderer::setCamera(Camera* camera)
{
	mCamera = camera;
}


void Renderer::addPointLight(PointLight light) 
{
	mPointLights.push_back(light);
}

void Renderer::addSpotLight(SpotLight light) 
{
	mSpotLights.push_back(light);
}

void Renderer::addDirectionalLight(DirectionalLight light)
{
	mDirectionalLight = light;
}

void Renderer::bindToShader(Material* mat)
{
	glUniform3fv(mat->viewPosLoc, 1, glm::value_ptr(mCamera->pos));
	glUniform3fv(mat->viewDirLoc, 1, glm::value_ptr(mCamera->front));

	glUniform3fv(mat->sunLightDir, 1, glm::value_ptr(mDirectionalLight.mDirection));
	glUniform3fv(mat->sunLightColor, 1, glm::value_ptr(mDirectionalLight.mColor));

	glUniform3fv(mat->fogColorLoc, 1, glm::value_ptr(fogColor));


	for (int i = 0; i < NUM_POINT_LIGHTS; i++) {

		PointLight p = PointLight(glm::vec3(0), glm::vec3(0));

		if (i < mPointLights.size()) {
			p = mPointLights[i];
		}

		glUniform3fv(mat->pointLightPosLoc[i], 1, glm::value_ptr(p.mPosition));
		glUniform3fv(mat->pointLightColorLoc[i], 1, glm::value_ptr(p.mColor));
		glUniform1f(mat->pointLightAttenConstLoc[i], p.mConstant);
		glUniform1f(mat->pointLightAttenLinearLoc[i], p.mLinear);
		glUniform1f(mat->pointLightAttenQuadLoc[i], p.mQuadratic);
	}

	for (int i = 0; i < NUM_SPOT_LIGHTS; i++) {

		// default point light which shouldn't contribute to lighting
		SpotLight s = SpotLight(glm::vec3(0), glm::vec3(0, 0, 1), glm::vec3(0), 45);

		if (i < mSpotLights.size()) {
			s = mSpotLights[i];
		}

		glUniform3fv(mat->spotLightPosLoc[i], 1, glm::value_ptr(s.mPosition));
		glUniform3fv(mat->spotLightColorLoc[i], 1, glm::value_ptr(s.mColor));
		glUniform1f(mat->spotLightAttenConstLoc[i], s.mConstant);
		glUniform1f(mat->spotLightAttenLinearLoc[i], s.mLinear);
		glUniform1f(mat->spotLightAttenQuadLoc[i], s.mQuadratic);
		glUniform1f(mat->spotLightAngleLoc[i], s.mAngle);
	}
}

void Renderer::update(double deltaTime)
{
	// todo: track time here for animation stuff
	gameTime += deltaTime;

	glClearColor(fogColor.x, fogColor.y, fogColor.z, 0);
}


PointLight::PointLight(glm::vec3 position, glm::vec3 color) {
	mPosition = position;
	mColor = color;

	// set some reasonable attenuation constants
	mConstant = 1.0f;
	mLinear = 0.01;
	mQuadratic = 0.001;
}

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color, float angle)
{
	mPosition = position;
	mDirection = direction;
	mColor = color;

	mAngle = angle;

	mConstant = 1.0f;
	mLinear = 0.01;
	mQuadratic = 0.001;
}
