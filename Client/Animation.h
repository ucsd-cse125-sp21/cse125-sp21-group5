#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <assimp/anim.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>
#include <iostream>
#include <cstdio>

#include "AssimpGLMHelpers.h"

//help recreate the heirarchy how the bones, like root -> head -> shoulder -> (chest, left arm, right arm)
//help pass down parent transforms.
//Will combine with the bone's own binding matrix offset and parent transform and local transform due to
//interpolation
class AssimpNodeData
{
public:
	glm::mat4 nodeTransformation;
	std::string nodeName; //will match the name of the bone, hopefully
	int childrenCount;
	std::vector<AssimpNodeData*> childrenNodeList;

	std::string parentNodeName = "";

	void setNodeData(aiNode* ainode) {
		if (ainode->mParent != NULL) {
			parentNodeName = ainode->mParent->mName.data;
		}
		nodeName = ainode->mName.data;
		childrenCount = ainode->mNumChildren;
		nodeTransformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(ainode->mTransformation);

		//std::cout << "(animation.h 41) Node name: " << nodeName <<  " ParentNode: "<< parentNodeName <<std::endl;
	}

	~AssimpNodeData() {
		for (AssimpNodeData* child : childrenNodeList)
		{
			delete child;
		}
	}
};


struct KeyPosition
{
	glm::vec3 position;
	float timeStamp;
};

struct KeyRotation
{
	glm::quat orientation;
	float timeStamp;
};

struct KeyScale
{
	glm::vec3 scale;
	float timeStamp;
};

//each channel will contain the keyframes information of a bone
//will have to check if the channel has a bone. CHANNEL NAME MUST BE SAME AS BONE
//IF A BONE DOES NOT EXIST, we have a problem. Something else got moved, extra movement not the bone
class Channel
{
public:
	std::string channelName; //can be the name of the bone/rig or something else like the whole mesh itself that was moved
	bool isAttachedtoBone;
	aiNodeAnim* ch; //this will contain a list of keyframes

	std::vector<KeyPosition> keyFrames_Position;
	std::vector<KeyRotation> keyFrames_Rotation;
	std::vector<KeyScale> keyFrames_Scale;
	int m_NumPositions; //tell the amount of key frame for each transformation per channel
	int m_NumRotations;
	int m_NumScalings;

	glm::mat4 m_LocalTransform; //the total transformation at a certain time in the animation (T * R * S)

	void loadKeyFrames() {
		//std::cout << "Loading the key frame of the channel (animation.h)" << std::endl;
		float timeStamp; //the time when the keyframe happen

		for (int i = 0; i < m_NumPositions; i++) {
			timeStamp = ch->mPositionKeys[i].mTime;
			KeyPosition kp = KeyPosition();
			kp.position = AssimpGLMHelpers::GetGLMVec(ch->mPositionKeys[i].mValue); //get the translation vector
			kp.timeStamp = timeStamp;
			
			keyFrames_Position.push_back(kp);
		}

		for (int i = 0; i < m_NumRotations; i++) {
			timeStamp = ch->mRotationKeys[i].mTime;
			KeyRotation kr = KeyRotation();
			kr.orientation = AssimpGLMHelpers::GetGLMQuat(ch->mRotationKeys[i].mValue);
			kr.timeStamp = timeStamp;

			keyFrames_Rotation.push_back(kr);
		}

		for (int i = 0; i < m_NumScalings; i++) {
			timeStamp = ch->mScalingKeys[i].mTime;
			KeyScale ks = KeyScale();
			ks.scale = AssimpGLMHelpers::GetGLMVec(ch->mScalingKeys[i].mValue); //get the translation vector
			ks.timeStamp = timeStamp;

			keyFrames_Scale.push_back(ks);
		}
	}

	void update(float currentTime);

	glm::mat4 getLocalTransform() {
		return m_LocalTransform;
	}
	//helper function
	/* Gets the current index on mKeyPositions to interpolate to based on the current 
	animation time */
	int GetPositionIndex(float animationTime)
	{
		for (int index = 0; index < m_NumPositions - 1; ++index)
		{
			if (animationTime < keyFrames_Position[index + 1].timeStamp) {
				return index;
			}
		}
		assert(0);
	}

	/* Gets the current index on mKeyRotations to interpolate to based on the current 
    animation time */
    int GetRotationIndex(float animationTime)
    {
        for (int index = 0; index < m_NumRotations - 1; ++index)
        {
            if (animationTime < keyFrames_Rotation[index + 1].timeStamp)
                return index;
        }
        assert(0);
    }

	/* Gets the current index on mKeyScalings to interpolate to based on the current 
    animation time */
    int GetScaleIndex(float animationTime)
    {
        for (int index = 0; index < m_NumScalings - 1; ++index)
        {
            if (animationTime < keyFrames_Scale[index + 1].timeStamp)
                return index;
        }
        assert(0);
    }


private:
	/* Gets normalized value for Lerp & Slerp*/
	float GetSlerpFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

    /* figures out which position keys to interpolate b/w and performs the interpolation 
    and returns the translation matrix */
	glm::mat4 InterpolatePosition(float animationTime);

    /* figures out which rotations keys to interpolate b/w and performs the interpolation 
    and returns the rotation matrix */
	glm::mat4 InterpolateRotation(float animationTime);

    /* figures out which scaling keys to interpolate b/w and performs the interpolation 
    and returns the scale matrix */
	glm::mat4 InterpolateScaling(float animationTime);
};

class Animation
{
private:

	std::vector<Channel> channelList; //maybe only add the channels attached to a bone

	aiNode* aiRoot; //will point to list of other aiNodes. Tell the skeleton of the bones

	AssimpNodeData* root;

	float mTicksPerSecond;
	float mDuration;

	/* Gets normalized value for Lerp & Slerp*/
    float GetSlerpFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
    {
        float scaleFactor = 0.0f;
        float midWayLength = animationTime - lastTimeStamp;
        float framesDiff = nextTimeStamp - lastTimeStamp;
        scaleFactor = midWayLength / framesDiff;
        return scaleFactor;
    }

public:

	Animation(aiAnimation* anim, aiNode* aiRootNode);
	~Animation();

	void ReadNodeHeirarchy(const aiNode* pNode, AssimpNodeData* asspNode); //a set-up fnc

	AssimpNodeData* getRootNode() {
		return root;
	}
	
	float getTicksPerSecond();
	float getDuration();

	Channel* findChannel(std::string nodeName);

};
