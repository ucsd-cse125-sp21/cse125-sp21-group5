#include "Material.h"

#include "Shader.h"
#include "stb_image.h"

Material::Material(std::string vertShaderPath, std::string fragShaderPath, aiMaterial* aiMat)
{
	shader = LoadShaders(vertShaderPath.c_str(), fragShaderPath.c_str());

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
		if (numChannels == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}

		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
}

Material::~Material()
{
	glDeleteTextures(1, &tex_diffuse);
}

void Material::activate()
{
	// activate the shader
	glUseProgram(shader);

	// activate the texture
	glBindTexture(GL_TEXTURE_2D, tex_diffuse);
}


void Material::release()
{
	glUseProgram(0);
}

