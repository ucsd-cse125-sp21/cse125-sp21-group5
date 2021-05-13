#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

#include <boost/serialization/vector.hpp>

class GameState
{
public:
	glm::vec3 pos, front;
	std::vector<float> flag1Pos;

	GameState(glm::vec3 pos, glm::vec3 front, std::vector<float> flag1Pos);
	GameState();

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& pos.x;
		ar& pos.y;
		ar& pos.z;
		ar& front.x;
		ar& front.y;
		ar& front.z;
		ar& flag1Pos;
	}
};