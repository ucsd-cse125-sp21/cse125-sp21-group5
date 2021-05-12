#pragma once

#include <vector>
#include "Animation.h"

#define NO_ANIMATION -1

#define MAX_NUM_BONES 100

class Model;

class AnimationPlayer
{
public:
	AnimationPlayer(std::vector<Animation*> animations, Model* model);

	void setAnimation(int newAnimationIdx);

	void update(float deltaTime);

	void calculateBoneTransform(const AssimpNodeData* node, const glm::mat4& parentTransform);

	std::vector<glm::mat4> getFinalBoneTransforms() {
		return mFinalBoneTransformationMatrices;
	}

private:

	std::vector<glm::mat4> mFinalBoneTransformationMatrices;

	Model* mModel;
	int mCurrentAnimationIdx = NO_ANIMATION;
	float mCurrentTime = 0;
	float mDeltaTime = 0;
	std::vector<Animation*> mAnimations;
};

