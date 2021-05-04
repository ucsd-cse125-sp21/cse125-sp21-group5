#pragma once

#include <vector>
#include "Animation.h"
#include "Model.h"

#define NO_ANIMATION -1

class AnimationPlayer
{
public:
	AnimationPlayer(std::vector<Animation*> animations, Model* model);

	void setAnimation(int newAnimationIdx);

	void update(float deltaTime);

	void calculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);

private:
	Model* mModel;
	int mCurrentAnimationIdx = NO_ANIMATION;
	float mCurrentTime = 0;
	float mDeltaTime = 0;
	std::vector<Animation*> mAnimations;
};

