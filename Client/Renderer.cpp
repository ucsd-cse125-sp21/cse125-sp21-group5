#include "Renderer.h"

#include "Shader.h"

void Renderer::setCamera(Camera* camera)
{
	mCamera = camera;
}


void Renderer::addPointLight(PointLight light) 
{
	mPointLights.push_back(light);
}

void Renderer::addDirectionalLight(DirectionalLight light)
{
	mDirectionalLight = light;
}


void Renderer::bindToShader(GLuint shader) 
{
	// tell the shader where we're viewing from
	SetShader3f(shader, "aViewPos", mCamera->pos);
	SetShader3f(shader, "aViewDir", mCamera->front);

	SetShader3f(shader, "sunLight.direction", mDirectionalLight.mDirection);
	SetShader3f(shader, "sunLight.color", mDirectionalLight.mColor);
	
	for (int i = 0; i < NUM_POINT_LIGHTS; i++) {

		// default point light which shouldn't contribute to lighting
		PointLight p = PointLight(glm::vec3(0), glm::vec3(0));

		if (i < mPointLights.size()) {
			p = mPointLights[i];
		}

		// write point light parameters to shader
		char buff[256];
		snprintf(buff, sizeof(buff), "pointlights[%d].%s", i, "position");
		SetShader3f(shader, buff, p.mPosition);

		snprintf(buff, sizeof(buff), "pointlights[%d].%s", i, "color");
		SetShader3f(shader, buff, p.mColor);

		snprintf(buff, sizeof(buff), "pointlights[%d].%s", i, "constant");
		SetShaderFloat(shader, buff, p.mConstant);

		snprintf(buff, sizeof(buff), "pointlights[%d].%s", i, "linear");
		SetShaderFloat(shader, buff, p.mLinear);

		snprintf(buff, sizeof(buff), "pointlights[%d].%s", i, "quadratic");
		SetShaderFloat(shader, buff, p.mQuadratic);
	}
}


PointLight::PointLight(glm::vec3 position, glm::vec3 color) {
	mPosition = position;
	mColor = color;

	// set some reasonable attenuation constants
	mConstant = 1.0f;
	mLinear = 0.01;
	mQuadratic = 0.001;
}
