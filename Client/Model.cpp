#include "Model.h"

#include "stb_image.h"

Model::Model(std::string modelPath)
{
	// loads the mesh into into position, normal, and index vectors
	loadModel(modelPath);
}

Model::~Model()
{
	for (Mesh* m : meshes) {
		delete m;
	}

	for (Material* m : materials) {
		delete m;
	}
}

void Model::update()
{
	
}

void Model::draw(const glm::mat4& modelMtx, const glm::mat4& viewProjMtx)
{

	for (Mesh* mesh : meshes) 
	{
		Material* mat = materials[mesh->materialIdx];
		mat->activate();

		// get the locations and send the uniforms to the shader 
		glUniformMatrix4fv(glGetUniformLocation(mat->shader, "viewProj"), 1, GL_FALSE, glm::value_ptr(viewProjMtx));
		glUniformMatrix4fv(glGetUniformLocation(mat->shader, "model"), 1, GL_FALSE, glm::value_ptr(model * modelMtx));

	    mesh->draw();
		mat->release();
	}
}

void Model::loadModel(std::string modelPath)
{
	Assimp::Importer import;

	unsigned int importOptions = aiProcess_Triangulate
		| aiProcess_JoinIdenticalVertices
		| aiProcess_FlipUVs
		| aiProcess_PreTransformVertices;

	const aiScene* scene = import.ReadFile(modelPath, importOptions);

	if (!scene) {
		std::cerr << "Failed to load model file " << modelPath << std::endl;
		return;
	}

	// Load materials
	for (int matidx = 0; matidx < scene->mNumMaterials; matidx++) {
		aiMaterial* aiMat = scene->mMaterials[matidx];

		aiString texturePath;
		if (AI_SUCCESS == aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath)) {
			// we found a textured material
			Material* mat = new TexturedMaterial(aiMat);
			materials.push_back(mat);
		}
		else {
			// found a diffuse texture
			Material* mat = new DiffuseMaterial(aiMat);
			materials.push_back(mat);
		}

	}

	// Load geometry meshes
	for (int meshidx = 0; meshidx < scene->mNumMeshes; meshidx++) {
		aiMesh* aiMesh = scene->mMeshes[meshidx];
		Mesh* mesh = new Mesh(aiMesh);
		meshes.push_back(mesh);
	}
}
