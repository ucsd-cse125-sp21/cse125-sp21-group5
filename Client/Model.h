#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>
#include <iostream>
#include <cstdio>

#include "Node.h"
#include "Mesh.h"
#include "Material.h"
#include "stb_image.h"

class Model : public Node
{
private:
	glm::mat4 model = glm::mat4(1.0f);

	// Stuff for textures
	std::vector<Mesh*> meshes;
	std::vector<Material*> materials;

	// Load the model's data from file
	void loadModel(std::string modelPath);

public:
	Model(std::string modelPath);
	~Model();

	void update();
	void draw(const glm::mat4& modelMtx, const glm::mat4& viewProjMtx);
};
