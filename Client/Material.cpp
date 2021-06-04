#include "Material.h"

#include <glm/gtx/string_cast.hpp>

#include "Shader.h"
#include "stb_image.h"
#include "Renderer.h"


void Material::loadShaderLocations() {

	viewProjLoc = glGetUniformLocation(shader, "aViewProj");
	viewPosLoc = glGetUniformLocation(shader, "aViewPos");
	viewDirLoc = glGetUniformLocation(shader, "aViewDir");

	sunLightDir = glGetUniformLocation(shader, "sunLight.direction");
	sunLightColor = glGetUniformLocation(shader, "sunLight.color");

	fogColorLoc = glGetUniformLocation(shader, "aFogColor");
	fogDensityLoc = glGetUniformLocation(shader, "aFogDensity");
	fogGradientLoc = glGetUniformLocation(shader, "aFogGradient");

	char buff[256];
	for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
		snprintf(buff, sizeof(buff), "pointlights[%d].%s", i, "position");
		pointLightPosLoc[i] = glGetUniformLocation(shader, buff);
		snprintf(buff, sizeof(buff), "pointlights[%d].%s", i, "color");
		pointLightColorLoc[i] = glGetUniformLocation(shader, buff);
		snprintf(buff, sizeof(buff), "pointlights[%d].%s", i, "constant");
		pointLightAttenConstLoc[i] = glGetUniformLocation(shader, buff);
		snprintf(buff, sizeof(buff), "pointlights[%d].%s", i, "linear");
		pointLightAttenLinearLoc[i] = glGetUniformLocation(shader, buff);
		snprintf(buff, sizeof(buff), "pointlights[%d].%s", i, "quadratic");
		pointLightAttenQuadLoc[i] = glGetUniformLocation(shader, buff);
	}

	for (int i = 0; i < NUM_SPOT_LIGHTS; i++) {
		snprintf(buff, sizeof(buff), "spotlights[%d].%s", i, "position");
		spotLightPosLoc[i] = glGetUniformLocation(shader, buff);
		snprintf(buff, sizeof(buff), "spotlights[%d].%s", i, "color");
		spotLightColorLoc[i] = glGetUniformLocation(shader, buff);
		snprintf(buff, sizeof(buff), "spotlights[%d].%s", i, "constant");
		spotLightAttenConstLoc[i] = glGetUniformLocation(shader, buff);
		snprintf(buff, sizeof(buff), "spotlights[%d].%s", i, "linear");
		spotLightAttenLinearLoc[i] = glGetUniformLocation(shader, buff);
		snprintf(buff, sizeof(buff), "spotlights[%d].%s", i, "quadratic");
		spotLightAttenQuadLoc[i] = glGetUniformLocation(shader, buff);
		snprintf(buff, sizeof(buff), "spotlights[%d].%s", i, "angle");
		spotLightAngleLoc[i] = glGetUniformLocation(shader, buff);
	}
}

TexturedMaterial::TexturedMaterial(aiMaterial* aiMat, std::string playerType) {
	shader = LoadShaders("res/shaders/texturedShader.vert", "res/shaders/texturedShader.frag");

	aiString path;
	aiReturn texFound = aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &path);

	char buff[100];
	snprintf(buff, sizeof(buff), "res/textures/%s/%s", playerType.c_str(), path.C_Str());
	//std::cout << "Loading from " << buff << std::endl;

	int width, height, numChannels;
	unsigned char* data = stbi_load(buff, &width, &height, &numChannels, 0);

	glGenTextures(1, &tex_diffuse);
	glBindTexture(GL_TEXTURE_2D, tex_diffuse);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	if (data) {
		// we need to handle images with transparency slightly differently from regular 3 channel images
		if (numChannels == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else if (numChannels == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}

		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}

	loadShaderLocations();
}


TexturedMaterial::TexturedMaterial(aiMaterial* aiMat)
{
	shader = LoadShaders("res/shaders/texturedShader.vert", "res/shaders/texturedShader.frag");

	aiString path;
	aiReturn texFound = aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &path);

	char buff[100];
	snprintf(buff, sizeof(buff), "res/textures/%s", path.C_Str());
	//std::cout << "Loading from " << buff << std::endl;

	int width, height, numChannels;
	unsigned char* data = stbi_load(buff, &width, &height, &numChannels, 0);

	glGenTextures(1, &tex_diffuse);
	glBindTexture(GL_TEXTURE_2D, tex_diffuse);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	if (data) {
		// we need to handle images with transparency slightly differently from regular 3 channel images
		if (numChannels == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else if (numChannels == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}

		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}

	loadShaderLocations();
}

TexturedMaterial::~TexturedMaterial()
{
	glDeleteTextures(1, &tex_diffuse);

	glDeleteProgram(shader);
}

void TexturedMaterial::activate()
{
	// activate the shader
	glUseProgram(shader);

	// activate the texture
	glBindTexture(GL_TEXTURE_2D, tex_diffuse);

	Renderer::get().bindToShader(this);
}


void TexturedMaterial::release()
{
	glUseProgram(0);
}

////// Start diffuse material code //////

DiffuseMaterial::DiffuseMaterial(aiMaterial* aiMat)
{
	shader = LoadShaders("res/shaders/diffuseShader.vert", "res/shaders/diffuseShader.frag");

	aiColor3D aiColor;
	aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
	diffuseColor = glm::vec3(aiColor.r, aiColor.g, aiColor.b);

	//std::cout << "Loaded diffuse material with color " << glm::to_string(diffuseColor) << std::endl;

	colorLocation = glGetUniformLocation(shader, "aColor");
	loadShaderLocations();
}

DiffuseMaterial::~DiffuseMaterial()
{
	glDeleteProgram(shader);
}

void DiffuseMaterial::activate()
{
	glUseProgram(shader);
	glUniform3fv(colorLocation, 1, glm::value_ptr(diffuseColor));
	Renderer::get().bindToShader(this);
}

void DiffuseMaterial::release()
{
	glUseProgram(0);
}

