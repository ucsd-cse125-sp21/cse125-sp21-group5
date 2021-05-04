#include "AnimationPlayer.h"

#include <GLFW/glfw3.h>

AnimationPlayer::AnimationPlayer(std::vector<Animation*> animations, Model* model)
{
	mAnimations = animations;
	mCurrentTime = glfwGetTime();
	mCurrentAnimationIdx = 0;

	mModel = model;
}

void AnimationPlayer::setAnimation(int newAnimationIdx)
{
	mCurrentAnimationIdx = newAnimationIdx;
	mCurrentTime = 0;
}

void AnimationPlayer::update(float deltaTime) 
{
	mDeltaTime = deltaTime;

	if (mCurrentAnimationIdx != NO_ANIMATION) {
		Animation* currAnimation = mAnimations[mCurrentAnimationIdx];
		mCurrentTime += currAnimation->getTicksPerSecond() * deltaTime;
		mCurrentTime = fmod(mCurrentTime, currAnimation->getDuration());

		calculateBoneTransform(currAnimation->getRootNode(), glm::mat4(1));
	}

}

void AnimationPlayer::calculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
{
	std::string nodeName = node->nodeName;
	glm::mat4 nodeTransform = node->nodeTransformation;

	Channel* channel = mAnimations[mCurrentAnimationIdx]->findChannel(nodeName);

	//make sure a node name and channel match, since there are extra nodes
	if (channel) {
		channel->update(mCurrentTime); //get the new matrix interpolated at a certain time for keyframe
		nodeTransform = channel->getLocalTransform(); 
	}

	glm::mat4 globalTransformation = parentTransform * nodeTransform; //pass along the tranform from the scene

	//for the bones, apply the binding matrix to it, to get into proper bone space
	//TO DO
	
	auto boneInfoMap = mModel->getBoneInfoMap();

	if (boneInfoMap.find(nodeName) != boneInfoMap.end()) {
		int index = boneInfoMap[nodeName].id;
		glm::mat4 offset = boneInfoMap[nodeName].offset;

	}

	//for (int i = 0; i < node->childrenCount; i++) {
	//	calculateBoneTransform(&node->children[i], globalTransformation);
	//}
}
