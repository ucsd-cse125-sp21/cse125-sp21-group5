#pragma once
#include <glm/glm.hpp>
#include <boost/serialization/vector.hpp>
#include <vector>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

using namespace std;

class MapState
{
public:
	int tileSeed;
	MapState();
	MapState(int seed);


	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& tileSeed;
	}
};

