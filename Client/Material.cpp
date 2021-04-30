#include "Material.h"

#include <glm/gtx/string_cast.hpp>

#include "Shader.h"
#include "stb_image.h"
#include "Renderer.h"

TexturedMaterial::TexturedMaterial(aiMaterial* aiMat)
{
	shader = LoadShaders("res/shaders/texturedShader.vert", "res/shaders/texturedShader.frag");

	aiString path;
	aiReturn texFound = aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &path);

	char buff[100];
	snprintf(buff, sizeof(buff), "res/textures/%s", path.C_Str());
	std::cout << "Loading from " << buff << std::endl;

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

	Renderer::get().bindToShader(shader);
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

	std::cout << "Loaded diffuse material with color " << glm::to_string(diffuseColor) << std::endl;

	colorLocation = glGetUniformLocation(shader, "aColor");
}

DiffuseMaterial::~DiffuseMaterial()
{
	glDeleteProgram(shader);
}

void DiffuseMaterial::activate()
{
	glUseProgram(shader);
	glUniform3fv(colorLocation, 1, glm::value_ptr(diffuseColor));
	Renderer::get().bindToShader(shader);
}

void DiffuseMaterial::release()
{
	glUseProgram(0);
}

