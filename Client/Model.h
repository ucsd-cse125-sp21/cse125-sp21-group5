#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>

#include "Mesh.h"
#include "Material.h"

class Model
{
private:

	std::vector<Mesh*> meshes;
	std::vector<Material*> materials;

	void loadModel(std::string modelPath); //read in the file, load the model's data

public:
	Model(std::string modelPath);
	~Model();

	void update();
	void draw(const glm::mat4& modelMtx, const glm::mat4& viewProjMtx);
};
