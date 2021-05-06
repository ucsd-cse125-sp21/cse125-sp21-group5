#include "Animation.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

Animation::Animation(aiAnimation* anim, aiNode* aiRootNode) {
	anime = anim;
	aiRoot = aiRootNode;

	//Set up the channels
	std::cout << "total channels: "<< anime->mNumChannels << std::endl;
	for (int chIdx = 0; chIdx < anime->mNumChannels; chIdx++) {
		aiNodeAnim* aiChannel = anime->mChannels[chIdx];
		std::cout << aiChannel->mNodeName.data << std::endl;

		Channel channel = Channel();
		channel.ch = aiChannel;
		channel.channelName = aiChannel->mNodeName.data;
		
		channel.m_NumPositions = aiChannel->mNumPositionKeys;
		channel.m_NumRotations = aiChannel->mNumRotationKeys;
		channel.m_NumScalings = aiChannel->mNumScalingKeys;

		channel.loadKeyFrames();

		channelList.push_back(channel);
	}

	//std::cout << "root Node name: "<< aiRoot->mName.data << std::endl;

	root = new AssimpNodeData();
	root->setNodeData(aiRoot);
	ReadNodeHeirarchy(aiRoot, root);
}

Animation::~Animation() {
	delete root;
}

void Animation::ReadNodeHeirarchy(const aiNode* pNode, AssimpNodeData* asspNode) {
	for (int i = 0; i < pNode->mNumChildren; i++) {
		aiNode* childNode = pNode->mChildren[i];
		AssimpNodeData* childAssimpNodeData = new AssimpNodeData();
		childAssimpNodeData->setNodeData(childNode); //set up the childNode data, assume parent already set up first thing
		asspNode->childrenNodeList.push_back(childAssimpNodeData);

		ReadNodeHeirarchy(childNode, childAssimpNodeData); //go thru the children's child list
	}
}

Channel* Animation::findChannel(std::string nodeName)
{
	for (int i = 0; i < channelList.size(); i++) {
		Channel* currChannel = &channelList[i];

		if (currChannel->channelName == nodeName) {
			return currChannel;
		}
	}

	return nullptr;
}

float Animation::getTicksPerSecond() {
	return anime->mTicksPerSecond;
}

float Animation::getDuration() { 
	return anime->mDuration;
}


///////// BEGIN CHANNEL CODE ///////// 

void Channel::update(float currentTime)
{
	glm::mat4 translation = InterpolatePosition(currentTime);
	glm::mat4 rotation = InterpolateRotation(currentTime);
	glm::mat4 scale = InterpolateScaling(currentTime);
	m_LocalTransform = translation * rotation * scale;
}

float Channel::GetSlerpFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
	float scaleFactor = 0.0f;
	float midWayLength = animationTime - lastTimeStamp;
	float framesDiff = nextTimeStamp - lastTimeStamp;
	scaleFactor = midWayLength / framesDiff;
	return scaleFactor;
}

glm::mat4 Channel::InterpolatePosition(float animationTime)
{
    if (1 == m_NumPositions) {
        return glm::translate(glm::mat4(1.0f), keyFrames_Position[0].position);
	}

	//to check betweeen two keyframes for interpolation values
    int p0Index = GetPositionIndex(animationTime);
    int p1Index = p0Index + 1; 

	float t0 = keyFrames_Position[p0Index].timeStamp;
	float t1 = keyFrames_Position[p1Index].timeStamp;
	glm::vec3 p0 = keyFrames_Position[p0Index].position;
	glm::vec3 p1 = keyFrames_Position[p1Index].position;
    float slerpFactor = GetSlerpFactor(t0, t1, animationTime);
    glm::vec3 finalPosition = glm::mix(p0, p1, slerpFactor);
    return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 Channel::InterpolateRotation(float animationTime)
{
	if (1 == m_NumRotations)
    {
        auto rotation = glm::normalize(keyFrames_Rotation[0].orientation);
        return glm::toMat4(rotation);
    }

	int p0Index = GetRotationIndex(animationTime);
    int p1Index = p0Index + 1;
	float t0 = keyFrames_Rotation[p0Index].timeStamp;
	float t1 = keyFrames_Rotation[p1Index].timeStamp;
	glm::quat p0 = keyFrames_Rotation[p0Index].orientation;
	glm::quat p1 = keyFrames_Rotation[p1Index].orientation;
	float slerpFactor = GetSlerpFactor(t0, t1, animationTime);
	glm::quat finalRotation = glm::slerp(p0, p1, slerpFactor);

	return glm::toMat4(finalRotation);
}

glm::mat4 Channel::InterpolateScaling(float animationTime)
{
	if (1 == m_NumScalings) {
		return glm::scale(glm::mat4(1.0f), keyFrames_Scale[0].scale);
	}
	int p0Index = GetScaleIndex(animationTime);
    int p1Index = p0Index + 1;
	float t0 = keyFrames_Scale[p0Index].timeStamp;
	float t1 = keyFrames_Scale[p1Index].timeStamp;
	glm::vec3 p0 = keyFrames_Scale[p0Index].scale;
	glm::vec3 p1 = keyFrames_Scale[p1Index].scale;
	float slerpFactor = GetSlerpFactor(t0, t1, animationTime);
	glm::vec3 finalScale = glm::mix(p0, p1, slerpFactor);
	return glm::scale(glm::mat4(1.0f), finalScale);
}
