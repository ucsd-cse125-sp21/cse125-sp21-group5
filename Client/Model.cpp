#include "Model.h"

#include "Shader.h"
#include "stb_image.h"
#include "Renderer.h"

#include <GLFW/glfw3.h>


Model::Model(std::string modelPath)
{
	// loads the mesh into into position, normal, and index vectors
	loadModel(modelPath);
	currTime = glfwGetTime();
	prevTime = glfwGetTime();
}

Model::~Model()
{
	for (Mesh* m : meshes) {
		delete m;
	}

	for (Material* m : materials) {
		delete m;
	}

	delete animationPlayer;
}

void Model::update(float deltaTime)
{
	//get the new transform of the bones
	if (animationPlayer != nullptr) animationPlayer->update(deltaTime);
}

void Model::draw(const glm::mat4& modelMtx, const glm::mat4& viewProjMtx)
{
	for (Mesh* mesh : meshes) 
	{
		Material* mat = materials[mesh->materialIdx];
		mat->activate();

		// set global transform matrices
		auto transforms = animationPlayer->getFinalBoneTransforms();

		for (int i = 0; i < transforms.size(); i++) {
			SetShaderMat4(mat->shader, "boneMatrices[" + std::to_string(i) + "]", transforms[i]);
		}

		// get the locations and send the uniforms to the shader 
		SetShaderMat4(mat->shader, "aViewProj", viewProjMtx);
		SetShader3f(mat->shader, "aViewPos", Renderer::get().mCamera->pos);
		SetShaderMat4(mat->shader, "aModel", modelMtx);

	    mesh->draw();
		mat->release();
	}
}

void Model::loadModel(std::string modelPath)
{
	Assimp::Importer import;

	// These are the old import options, just to refer back to incase something breaks
	//unsigned int importOptions = 
	//	  aiProcess_Triangulate
	//	| aiProcess_JoinIdenticalVertices
	//	| aiProcess_FlipUVs
	//	| aiProcess_LimitBoneWeights;

	unsigned int importOptions = 
		aiProcess_JoinIdenticalVertices |		// join identical vertices/ optimize indexing
		aiProcess_ValidateDataStructure |		// perform a full validation of the loader's output
		aiProcess_ImproveCacheLocality |		// improve the cache locality of the output vertices
		aiProcess_RemoveRedundantMaterials |	// remove redundant materials
		aiProcess_GenUVCoords |					// convert spherical, cylindrical, box and planar mapping to proper UVs
		aiProcess_TransformUVCoords |			// pre-process UV transformations (scaling, translation ...)
		aiProcess_LimitBoneWeights |			// limit bone weights to 4 per vertex
		//aiProcess_OptimizeMeshes |				// join small meshes, if possible;
		//aiProcess_PreTransformVertices |
		aiProcess_FlipUVs |
		aiProcess_GenSmoothNormals |			// generate smooth normal vectors if not existing
		aiProcess_SplitLargeMeshes |			// split large, unrenderable meshes into sub-meshes
		aiProcess_Triangulate |					// triangulate polygons with more than 3 edges
		//aiProcess_ConvertToLeftHanded |			// convert everything to D3D left handed space
		aiProcess_SortByPType;

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
		Mesh* mesh = new Mesh(aiMesh, this);
		meshes.push_back(mesh);

		int numBone = aiMesh->mNumBones;
		printf("nmesh %d\n", meshidx);
		printf("numBone %d\n", numBone);

		for (int boneIdx = 0; boneIdx < numBone; boneIdx++) {
			//Each bone has a list of pair (vertexId, weight value). The vertex id
			//points to the index in the vertice list of the mesh. Th weights affect
			//that vertex. A bone also has an offest matrix which is the transformation 
			//from local space to the bone space, the binding matrix. Each bone hasits own shape

			aiBone* aiBone = aiMesh->mBones[boneIdx];
			std::string aiboneName = aiBone->mName.data;

			Bone* bone = new Bone(aiBone);
			mesh->boneList.push_back(bone);

			//this part is just storing the unique bone of the model into a map.
			//The key is the bone name
			//The value is a BoneInfo obj that contains the binding matrix, and a id (using the amount of bone from 0 - totalBoneNum - 1)
			int boneID = -1;
			if (m_BoneInfoMap.find(aiboneName) == m_BoneInfoMap.end())
			{
				BoneInfo newBoneInfo; //struct from Model.h
				newBoneInfo.id = m_BoneCounter;
				newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(aiBone->mOffsetMatrix);

				m_BoneInfoMap[aiboneName] = newBoneInfo;
				boneID = m_BoneCounter;
				m_BoneCounter++;
			}
			else
			{
				boneID = m_BoneInfoMap[aiboneName].id;
			}
			assert(boneID != -1);

			aiVertexWeight* weights = aiBone->mWeights;
			int numWeights = aiBone->mNumWeights;

            for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
            {
                int vertexId = weights[weightIndex].mVertexId;
                float weight = weights[weightIndex].mWeight;
                assert(vertexId <= mesh->vertices.size());
				mesh->vertices[vertexId].addBone(boneID, weight);
                //SetVertexBoneData(vertices[vertexId], boneID, weight);
            }
		}
		mesh->setupOpenGL();
	}

	//printf("UH %d\n", scene->HasAnimations());
	//printf("total unique bones %d\n", m_BoneInfoMap.size());
	printf("total unique bones %d\n", m_BoneCounter);
	//printf("number of Animation %d\n", scene->mNumAnimations);
	aiNode* rootNode = scene->mRootNode;

	glm::mat4 globalTrans = AssimpGLMHelpers::ConvertMatrixToGLMFormat(rootNode->mTransformation);
	glm::mat4 globalInvTrans = inverse(globalTrans);

	std::vector<Animation*> animationList;

	//Load animations
	for (int animIdx = 0; animIdx < scene->mNumAnimations; animIdx++) {
		aiAnimation* aiAnim = scene->mAnimations[animIdx];

		Animation* anime = new Animation(aiAnim, rootNode);
		animationList.push_back(anime);
		//printf("numChannels: %d\n", aiAnim->mNumChannels);
		//printf("duration: %lf\n", aiAnim->mDuration);
		//printf("meshChannel: %d\n", aiAnim->mNumMeshChannels);
		
	}

	animationPlayer = new AnimationPlayer(animationList, this);
}
