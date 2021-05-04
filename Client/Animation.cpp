#include "Animation.h"

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

	root = AssimpNodeData();
	root.setNodeData(aiRoot);
	ReadNodeHeirarchy(aiRoot, root);
}

Animation::~Animation() {

}

void Animation::ReadNodeHeirarchy(const aiNode* pNode, AssimpNodeData& asspNode) {
	for (int i = 0; i < pNode->mNumChildren; i++) {
		aiNode* childNode = pNode->mChildren[i];
		AssimpNodeData childAssimpNodeData = AssimpNodeData();
		childAssimpNodeData.setNodeData(childNode); //set up the childNode data, assume parent already set up first thing
		asspNode.childrenNodeList.push_back(childAssimpNodeData);

		ReadNodeHeirarchy(childNode, childAssimpNodeData); //go thru the children's child list
	}
}

Channel* Animation::findChannel(std::string nodeName)
{
	for (int i = 0; i < channelList.size(); i++) {
		Channel* currChannel = &channelList[i];

		if (currChannel->channelName == nodeName) {
			// todo: remove this print
			std::cout << "Found matching channel named " << nodeName << std::endl;
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

void Channel::update(float currentTime)
{
}
