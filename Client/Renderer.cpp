#include "Renderer.h"

#include "Shader.h"

void Renderer::setCamera(Camera* camera)
{
	mCamera = camera;
}


void Renderer::addLight(PointLight light) 
{
	mPointLights.push_back(light);
}


void Renderer::bindToShader(GLuint shader) 
{
	// tell the shader where we're viewing from
	SetShader3f(shader, "aViewPos", mCamera->pos);
	SetShader3f(shader, "aViewDir", mCamera->front);
	

	for (int i = 0; i < NUM_POINT_LIGHTS; i++) {

		// default point light which shouldn't contribute to lighting
		PointLight p = PointLight(glm::vec3(0), glm::vec3(0));

		if (i < mPointLights.size()) {
			p = mPointLights[i];
		}

		// write point light parameters to shader
		char buff[256];
		snprintf(buff, sizeof(buff), "pointlights[%d].%s", i, "position");
		SetShader3f(shader, buff, p.position);

		snprintf(buff, sizeof(buff), "pointlights[%d].%s", i, "color");
		SetShader3f(shader, buff, p.color);

		snprintf(buff, sizeof(buff), "pointlights[%d].%s", i, "constant");
		SetShaderFloat(shader, buff, p.constant);

		snprintf(buff, sizeof(buff), "pointlights[%d].%s", i, "linear");
		SetShaderFloat(shader, buff, p.linear);

		snprintf(buff, sizeof(buff), "pointlights[%d].%s", i, "quadratic");
		SetShaderFloat(shader, buff, p.quadratic);
	}
}


PointLight::PointLight(glm::vec3 position, glm::vec3 color) {
	this->position = position;
	this->color = color;

	// set some reasonable attenuation constants
	constant = 1.0f;
	linear = 0.01;
	quadratic = 0.001;
}
