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

#include <map>
#include "Bone.h"
#include "Animation.h"

struct BoneInfo
{
	/*id is index in finalBoneMatrices*/
	int id; //we will use the counter for the bone

	/*offset matrix transforms vertex from model space to bone space*/
	glm::mat4 offset; //binding matrix

};

class Model : public Node
{
private:
	// Stuff for textures
	std::vector<Mesh*> meshes;
	std::vector<Material*> materials;

	std::vector<Animation*> animationList;
	std::map<std::string, BoneInfo> m_BoneInfoMap; //help sum up all the weights
	int m_BoneCounter = 0;

	void loadModel(std::string modelPath); //read in the file, load the model's data

public:
	Model(std::string modelPath);
	~Model();

	void update();
	void draw(const glm::mat4& modelMtx, const glm::mat4& viewProjMtx);
};


class AnimatedModel : private Model
{
public:


};
