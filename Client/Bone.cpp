#include "Bone.h"
#include <iostream>

Bone::Bone(aiBone* b) {
	aibone = b;
	boneName = aibone->mName.data;
	offsetBoneMatrix = AssimpGLMHelpers::ConvertMatrixToGLMFormat(aibone->mOffsetMatrix);
	aiVertexWeight* aiVertexWeightList = aibone->mWeights;

	std::cout << "Bone name: " << boneName << std::endl;
	for (int i = 0; i < aibone->mNumWeights; i++) {
		int vertexIndex = aiVertexWeightList[i].mVertexId;
		float weight = aiVertexWeightList[i].mWeight;

		//std::cout << "vertexIndex: " << vertexIndex <<  " weight: "<<  weight <<std::endl;
		//std::cout << weight << std::endl;

		vertexIDList.push_back(vertexIndex);
		weightList.push_back(weight);
	}
}


Bone::~Bone() {

}

//for each VertIdx: vertex in a mesh
	//loop thru each bone in the boneList
		//get only the vertex affected by that bone, bone vertexIndx[vertIdx] is equal to vertIdx

//or For each bone in the mesh
	//loop thru its list of vertex, push the weight associated with that vertex???
